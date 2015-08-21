/*
xxHash - Fast Hash algorithm
Copyright (C) 2012-2014, Yann Collet.
BSD 2-Clause License (http://www.opensource.org/licenses/bsd-license.php)

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above
copyright notice, this list of conditions and the following disclaimer
in the documentation and/or other materials provided with the
distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

You can contact the author at :
- xxHash source repository : http://code.google.com/p/xxhash/
*/

#include "precompiled.h"

//**************************************
// Tuning parameters
//**************************************
// Unaligned memory access is automatically enabled for "common" CPU, such as x86.
// For others CPU, the compiler will be more cautious, and insert extra code to ensure aligned access is respected.
// If you know your target CPU supports unaligned memory access, you want to force this option manually to improve performance.
// You can also enable this parameter if you know your input data will always be aligned (boundaries of 4, for U32).
#if defined(__ARM_FEATURE_UNALIGNED) || defined(__i386) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_X64)
#  define DXM_XXH_USE_UNALIGNED_ACCESS 1
#endif

// DXM_XXH_ACCEPT_NULL_INPUT_POINTER :
// If the input pointer is a null pointer, xxHash default behavior is to trigger a memory access error, since it is a bad pointer.
// When this option is enabled, xxHash output for null input pointers will be the same as a null-length input.
// This option has a very small performance cost (only measurable on small inputs).
// By default, this option is disabled. To enable it, uncomment below define :
//#define DXM_XXH_ACCEPT_NULL_INPUT_POINTER 1

// DXM_XXH_FORCE_NATIVE_FORMAT :
// By default, xxHash library provides endian-independant Hash values, based on little-endian convention.
// Results are therefore identical for little-endian and big-endian CPU.
// This comes at a performance cost for big-endian CPU, since some swapping is required to emulate little-endian format.
// Should endian-independance be of no importance for your application, you may set the #define below to 1.
// It will improve speed for Big-endian CPU.
// This option has no impact on Little_Endian CPU.
#define DXM_XXH_FORCE_NATIVE_FORMAT 0


//**************************************
// Compiler Specific Options
//**************************************
// Disable some Visual warning messages
#ifdef _MSC_VER  // Visual Studio
#  pragma warning(disable : 4127)      // disable: C4127: conditional expression is constant
#endif

#ifdef _MSC_VER    // Visual Studio
#  define FORCE_INLINE static __forceinline
#else 
#  ifdef __GNUC__
#    define FORCE_INLINE static inline __attribute__((always_inline))
#  else
#    define FORCE_INLINE static inline
#  endif
#endif


//**************************************
// Includes & Memory related functions
//**************************************
#include "dxm_xxhash.h"
// Modify the local functions below should you wish to use some other memory related routines
// for malloc(), free()
#include <stdlib.h>
FORCE_INLINE void* DXM_XXH_malloc(size_t s) { return malloc(s); }
FORCE_INLINE void  DXM_XXH_free  (void* p)  { free(p); }
// for memcpy()
#include <string.h>
FORCE_INLINE void* DXM_XXH_memcpy(void* dest, const void* src, size_t size) { return memcpy(dest,src,size); }


//**************************************
// Basic Types
//**************************************
#if defined (__STDC_VERSION__) && __STDC_VERSION__ >= 199901L   // C99
# include <stdint.h>
  typedef uint8_t  BYTE;
  typedef uint16_t U16;
  typedef uint32_t U32;
  typedef  int32_t S32;
  typedef uint64_t U64;
#else
  typedef unsigned char      BYTE;
  typedef unsigned short     U16;
  typedef unsigned int       U32;
  typedef   signed int       S32;
  typedef unsigned long long U64;
#endif

#if defined(__GNUC__)  && !defined(DXM_XXH_USE_UNALIGNED_ACCESS)
#  define _PACKED __attribute__ ((packed))
#else
#  define _PACKED
#endif

#if !defined(DXM_XXH_USE_UNALIGNED_ACCESS) && !defined(__GNUC__)
#  ifdef __IBMC__
#    pragma pack(1)
#  else
#    pragma pack(push, 1)
#  endif
#endif

typedef struct _U32_S { U32 v; } _PACKED U32_S;

#if !defined(DXM_XXH_USE_UNALIGNED_ACCESS) && !defined(__GNUC__)
#  pragma pack(pop)
#endif

#define A32(x) (((U32_S *)(x))->v)


//***************************************
// Compiler-specific Functions and Macros
//***************************************
#define GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)

// Note : although _rotl exists for minGW (GCC under windows), performance seems poor
#if defined(_MSC_VER)
#  define DXM_XXH_rotl32(x,r) _rotl(x,r)
#else
#  define DXM_XXH_rotl32(x,r) ((x << r) | (x >> (32 - r)))
#endif

#if defined(_MSC_VER)     // Visual Studio
#  define DXM_XXH_swap32 _byteswap_ulong
#elif GCC_VERSION >= 403
#  define DXM_XXH_swap32 __builtin_bswap32
#else
static inline U32 DXM_XXH_swap32 (U32 x) {
    return  ((x << 24) & 0xff000000 ) |
        ((x <<  8) & 0x00ff0000 ) |
        ((x >>  8) & 0x0000ff00 ) |
        ((x >> 24) & 0x000000ff );}
#endif


//**************************************
// Constants
//**************************************
#define PRIME32_1   2654435761U
#define PRIME32_2   2246822519U
#define PRIME32_3   3266489917U
#define PRIME32_4    668265263U
#define PRIME32_5    374761393U


//**************************************
// Architecture Macros
//**************************************
typedef enum { DXM_XXH_bigEndian=0, DXM_XXH_littleEndian=1 } DXM_XXH_endianess;
#ifndef DXM_XXH_CPU_LITTLE_ENDIAN   // It is possible to define DXM_XXH_CPU_LITTLE_ENDIAN externally, for example using a compiler switch
    static const int one = 1;
#   define DXM_XXH_CPU_LITTLE_ENDIAN   (*(char*)(&one))
#endif


//**************************************
// Macros
//**************************************
#define DXM_XXH_STATIC_ASSERT(c)   { enum { DXM_XXH_static_assert = 1/(!!(c)) }; }    // use only *after* variable declarations


//****************************
// Memory reads
//****************************
typedef enum { DXM_XXH_aligned, DXM_XXH_unaligned } DXM_XXH_alignment;

FORCE_INLINE U32 DXM_XXH_readLE32_align(const U32* ptr, DXM_XXH_endianess endian, DXM_XXH_alignment align)
{ 
    if (align==DXM_XXH_unaligned)
        return endian==DXM_XXH_littleEndian ? A32(ptr) : DXM_XXH_swap32(A32(ptr)); 
    else
        return endian==DXM_XXH_littleEndian ? *ptr : DXM_XXH_swap32(*ptr); 
}

FORCE_INLINE U32 DXM_XXH_readLE32(const U32* ptr, DXM_XXH_endianess endian) { return DXM_XXH_readLE32_align(ptr, endian, DXM_XXH_unaligned); }


//****************************
// Simple Hash Functions
//****************************
FORCE_INLINE U32 DXM_XXH32_endian_align(const void* input, int len, U32 seed, DXM_XXH_endianess endian, DXM_XXH_alignment align)
{
    const BYTE* p = (const BYTE*)input;
    const BYTE* const bEnd = p + len;
    U32 h32;

#ifdef DXM_XXH_ACCEPT_NULL_INPUT_POINTER
    if (p==NULL) { len=0; p=(const BYTE*)(size_t)16; }
#endif

    if (len>=16)
    {
        const BYTE* const limit = bEnd - 16;
        U32 v1 = seed + PRIME32_1 + PRIME32_2;
        U32 v2 = seed + PRIME32_2;
        U32 v3 = seed + 0;
        U32 v4 = seed - PRIME32_1;

        do
        {
            v1 += DXM_XXH_readLE32_align((const U32*)p, endian, align) * PRIME32_2; v1 = DXM_XXH_rotl32(v1, 13); v1 *= PRIME32_1; p+=4;
            v2 += DXM_XXH_readLE32_align((const U32*)p, endian, align) * PRIME32_2; v2 = DXM_XXH_rotl32(v2, 13); v2 *= PRIME32_1; p+=4;
            v3 += DXM_XXH_readLE32_align((const U32*)p, endian, align) * PRIME32_2; v3 = DXM_XXH_rotl32(v3, 13); v3 *= PRIME32_1; p+=4;
            v4 += DXM_XXH_readLE32_align((const U32*)p, endian, align) * PRIME32_2; v4 = DXM_XXH_rotl32(v4, 13); v4 *= PRIME32_1; p+=4;
        } while (p<=limit);

        h32 = DXM_XXH_rotl32(v1, 1) + DXM_XXH_rotl32(v2, 7) + DXM_XXH_rotl32(v3, 12) + DXM_XXH_rotl32(v4, 18);
    }
    else
    {
        h32  = seed + PRIME32_5;
    }

    h32 += (U32) len;

    while (p<=bEnd-4)
    {
        h32 += DXM_XXH_readLE32_align((const U32*)p, endian, align) * PRIME32_3;
        h32  = DXM_XXH_rotl32(h32, 17) * PRIME32_4 ;
        p+=4;
    }

    while (p<bEnd)
    {
        h32 += (*p) * PRIME32_5;
        h32 = DXM_XXH_rotl32(h32, 11) * PRIME32_1 ;
        p++;
    }

    h32 ^= h32 >> 15;
    h32 *= PRIME32_2;
    h32 ^= h32 >> 13;
    h32 *= PRIME32_3;
    h32 ^= h32 >> 16;

    return h32;
}


U32 DXM_XXH32(const void* input, int len, U32 seed)
{
#if 0
    // Simple version, good for code maintenance, but unfortunately slow for small inputs
    void* state = DXM_XXH32_init(seed);
    DXM_XXH32_update(state, input, len);
    return DXM_XXH32_digest(state);
#else
    DXM_XXH_endianess endian_detected = (DXM_XXH_endianess)DXM_XXH_CPU_LITTLE_ENDIAN;

#  if !defined(DXM_XXH_USE_UNALIGNED_ACCESS)
    if ((((size_t)input) & 3))   // Input is aligned, let's leverage the speed advantage
    {
        if ((endian_detected==DXM_XXH_littleEndian) || DXM_XXH_FORCE_NATIVE_FORMAT)
            return DXM_XXH32_endian_align(input, len, seed, DXM_XXH_littleEndian, DXM_XXH_aligned);
        else
            return DXM_XXH32_endian_align(input, len, seed, DXM_XXH_bigEndian, DXM_XXH_aligned);
    }
#  endif

    if ((endian_detected==DXM_XXH_littleEndian) || DXM_XXH_FORCE_NATIVE_FORMAT)
        return DXM_XXH32_endian_align(input, len, seed, DXM_XXH_littleEndian, DXM_XXH_unaligned);
    else
        return DXM_XXH32_endian_align(input, len, seed, DXM_XXH_bigEndian, DXM_XXH_unaligned);
#endif
}


//****************************
// Advanced Hash Functions
//****************************

struct DXM_XXH_state32_t
{
    U64 total_len;
    U32 seed;
    U32 v1;
    U32 v2;
    U32 v3;
    U32 v4;
    int memsize;
    char memory[16];
};


int DXM_XXH32_sizeofState() 
{
    // DXM_XXH_STATIC_ASSERT( DXM_XXH32_SIZEOFSTATE >= sizeof(struct DXM_XXH_state32_t) );   // A compilation error here means DXM_XXH32_SIZEOFSTATE is not large enough
    return sizeof(struct DXM_XXH_state32_t); 
}


DXM_XXH_errorcode DXM_XXH32_resetState(void* state_in, U32 seed)
{ 
    struct DXM_XXH_state32_t * state = (struct DXM_XXH_state32_t *) state_in;
    state->seed = seed;
    state->v1 = seed + PRIME32_1 + PRIME32_2;
    state->v2 = seed + PRIME32_2;
    state->v3 = seed + 0;
    state->v4 = seed - PRIME32_1;
    state->total_len = 0;
    state->memsize = 0;
    return DXM_XXH_OK;
}


void* DXM_XXH32_init (U32 seed)
{
    void* state = DXM_XXH_malloc (sizeof(struct DXM_XXH_state32_t));
    DXM_XXH32_resetState(state, seed);
    return state;
}


FORCE_INLINE DXM_XXH_errorcode DXM_XXH32_update_endian (void* state_in, const void* input, int len, DXM_XXH_endianess endian)
{
    struct DXM_XXH_state32_t * state = (struct DXM_XXH_state32_t *) state_in;
    const BYTE* p = (const BYTE*)input;
    const BYTE* const bEnd = p + len;

#ifdef DXM_XXH_ACCEPT_NULL_INPUT_POINTER
    if (input==NULL) return DXM_XXH_ERROR;
#endif

    state->total_len += len;

    if (state->memsize + len < 16)   // fill in tmp buffer
    {
        DXM_XXH_memcpy(state->memory + state->memsize, input, len);
        state->memsize +=  len;
        return DXM_XXH_OK;
    }

    if (state->memsize)   // some data left from previous update
    {
        DXM_XXH_memcpy(state->memory + state->memsize, input, 16-state->memsize);
        {
            const U32* p32 = (const U32*)state->memory;
            state->v1 += DXM_XXH_readLE32(p32, endian) * PRIME32_2; state->v1 = DXM_XXH_rotl32(state->v1, 13); state->v1 *= PRIME32_1; p32++;
            state->v2 += DXM_XXH_readLE32(p32, endian) * PRIME32_2; state->v2 = DXM_XXH_rotl32(state->v2, 13); state->v2 *= PRIME32_1; p32++; 
            state->v3 += DXM_XXH_readLE32(p32, endian) * PRIME32_2; state->v3 = DXM_XXH_rotl32(state->v3, 13); state->v3 *= PRIME32_1; p32++;
            state->v4 += DXM_XXH_readLE32(p32, endian) * PRIME32_2; state->v4 = DXM_XXH_rotl32(state->v4, 13); state->v4 *= PRIME32_1; p32++;
        }
        p += 16-state->memsize;
        state->memsize = 0;
    }

    if (p <= bEnd-16)
    {
        const BYTE* const limit = bEnd - 16;
        U32 v1 = state->v1;
        U32 v2 = state->v2;
        U32 v3 = state->v3;
        U32 v4 = state->v4;

        do
        {
            v1 += DXM_XXH_readLE32((const U32*)p, endian) * PRIME32_2; v1 = DXM_XXH_rotl32(v1, 13); v1 *= PRIME32_1; p+=4;
            v2 += DXM_XXH_readLE32((const U32*)p, endian) * PRIME32_2; v2 = DXM_XXH_rotl32(v2, 13); v2 *= PRIME32_1; p+=4;
            v3 += DXM_XXH_readLE32((const U32*)p, endian) * PRIME32_2; v3 = DXM_XXH_rotl32(v3, 13); v3 *= PRIME32_1; p+=4;
            v4 += DXM_XXH_readLE32((const U32*)p, endian) * PRIME32_2; v4 = DXM_XXH_rotl32(v4, 13); v4 *= PRIME32_1; p+=4;
        } while (p<=limit);

        state->v1 = v1;
        state->v2 = v2;
        state->v3 = v3;
        state->v4 = v4;
    }

    if (p < bEnd)
    {
        DXM_XXH_memcpy(state->memory, p, bEnd-p);
        state->memsize = (int)(bEnd-p);
    }

    return DXM_XXH_OK;
}

DXM_XXH_errorcode DXM_XXH32_update (void* state_in, const void* input, int len)
{
    DXM_XXH_endianess endian_detected = (DXM_XXH_endianess)DXM_XXH_CPU_LITTLE_ENDIAN;
    
    if ((endian_detected==DXM_XXH_littleEndian) || DXM_XXH_FORCE_NATIVE_FORMAT)
        return DXM_XXH32_update_endian(state_in, input, len, DXM_XXH_littleEndian);
    else
        return DXM_XXH32_update_endian(state_in, input, len, DXM_XXH_bigEndian);
}



FORCE_INLINE U32 DXM_XXH32_intermediateDigest_endian (void* state_in, DXM_XXH_endianess endian)
{
    struct DXM_XXH_state32_t * state = (struct DXM_XXH_state32_t *) state_in;
    const BYTE * p = (const BYTE*)state->memory;
    BYTE* bEnd = (BYTE*)state->memory + state->memsize;
    U32 h32;

    if (state->total_len >= 16)
    {
        h32 = DXM_XXH_rotl32(state->v1, 1) + DXM_XXH_rotl32(state->v2, 7) + DXM_XXH_rotl32(state->v3, 12) + DXM_XXH_rotl32(state->v4, 18);
    }
    else
    {
        h32  = state->seed + PRIME32_5;
    }

    h32 += (U32) state->total_len;

    while (p<=bEnd-4)
    {
        h32 += DXM_XXH_readLE32((const U32*)p, endian) * PRIME32_3;
        h32  = DXM_XXH_rotl32(h32, 17) * PRIME32_4;
        p+=4;
    }

    while (p<bEnd)
    {
        h32 += (*p) * PRIME32_5;
        h32 = DXM_XXH_rotl32(h32, 11) * PRIME32_1;
        p++;
    }

    h32 ^= h32 >> 15;
    h32 *= PRIME32_2;
    h32 ^= h32 >> 13;
    h32 *= PRIME32_3;
    h32 ^= h32 >> 16;

    return h32;
}


U32 DXM_XXH32_intermediateDigest (void* state_in)
{
    DXM_XXH_endianess endian_detected = (DXM_XXH_endianess)DXM_XXH_CPU_LITTLE_ENDIAN;
    
    if ((endian_detected==DXM_XXH_littleEndian) || DXM_XXH_FORCE_NATIVE_FORMAT)
        return DXM_XXH32_intermediateDigest_endian(state_in, DXM_XXH_littleEndian);
    else
        return DXM_XXH32_intermediateDigest_endian(state_in, DXM_XXH_bigEndian);
}


U32 DXM_XXH32_digest (void* state_in)
{
    U32 h32 = DXM_XXH32_intermediateDigest(state_in);

    DXM_XXH_free(state_in);

    return h32;
}

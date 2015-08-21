
#ifndef __DXM_PLATFORM_CONFIG_H__
#define __DXM_PLATFORM_CONFIG_H__

//////////////////////////////////////////////////////////////////////////
// pre configure
//////////////////////////////////////////////////////////////////////////

// define supported target platform macro which DXM uses.
#define DXM_PLATFORM_UNKNOWN            0
#define DXM_PLATFORM_IOS                1
#define DXM_PLATFORM_ANDROID            2
#define DXM_PLATFORM_WIN32              3
#define DXM_PLATFORM_MARMALADE          4
#define DXM_PLATFORM_LINUX              5
#define DXM_PLATFORM_BADA               6
#define DXM_PLATFORM_BLACKBERRY         7
#define DXM_PLATFORM_MAC                8
#define DXM_PLATFORM_FREEBSD            9

// Determine target platform by compile environment macro.
#define DXM_TARGET_PLATFORM             DXM_PLATFORM_UNKNOWN

// iphone
#if ! DXM_TARGET_PLATFORM && (defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR))
#undef  DXM_TARGET_PLATFORM
#define DXM_TARGET_PLATFORM         DXM_PLATFORM_IOS
#define DXM_SUPPORT_PVRTC
#endif

// android
#if ! DXM_TARGET_PLATFORM && defined(ANDROID)
#undef  DXM_TARGET_PLATFORM
#define DXM_TARGET_PLATFORM         DXM_PLATFORM_ANDROID
#endif

// win32
#if ! DXM_TARGET_PLATFORM && (defined(WIN32) || defined(_WIN64))
#undef  DXM_TARGET_PLATFORM
#define DXM_TARGET_PLATFORM         DXM_PLATFORM_WIN32
#endif

// linux
#if ! DXM_TARGET_PLATFORM && defined(__linux__)
#undef  DXM_TARGET_PLATFORM
#define DXM_TARGET_PLATFORM         DXM_PLATFORM_LINUX
#endif

// marmalade
#if ! DXM_TARGET_PLATFORM && defined(MARMALADE)
#undef  DXM_TARGET_PLATFORM
#define DXM_TARGET_PLATFORM         DXM_PLATFORM_MARMALADE
#endif

// bada
#if ! DXM_TARGET_PLATFORM && defined(SHP)
#undef  DXM_TARGET_PLATFORM
#define DXM_TARGET_PLATFORM         DXM_PLATFORM_BADA
#endif

// qnx
#if ! DXM_TARGET_PLATFORM && defined(__QNX__)
#undef  DXM_TARGET_PLATFORM
#define DXM_TARGET_PLATFORM     DXM_PLATFORM_BLACKBERRY
#endif

// mac
#if ! DXM_TARGET_PLATFORM && defined(TARGET_OS_MAC)
#undef  DXM_TARGET_PLATFORM
#define DXM_TARGET_PLATFORM         DXM_PLATFORM_MAC
//#define DXM_SUPPORT_PVRTC
#endif

//////////////////////////////////////////////////////////////////////////
// post configure
//////////////////////////////////////////////////////////////////////////

// check user set platform
#if ! DXM_TARGET_PLATFORM
#error  "Cannot recognize the target platform; are you targeting an unsupported platform?"
#endif 

#if (DXM_TARGET_PLATFORM == DXM_PLATFORM_WIN32)
#pragma warning(disable:4127)  
#pragma warning(disable:4251)
#pragma warning(disable:4996)
#pragma warning(disable:4244)
#pragma warning(disable:4018)
#pragma warning(disable:4275)
#pragma warning(disable:4344)
#pragma warning(disable:4819)
#endif  // DXM_PLATFORM_WIN32

#endif  // __DXM_PLATFORM_CONFIG_H__


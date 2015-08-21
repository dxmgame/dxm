#ifndef __DXM_UTIL_CRYPTO_H__
#define __DXM_UTIL_CRYPTO_H__

#include "dxm_util_lib.h"
#include <string>

NS_DXM_BEGIN
NS_UTIL_BEGIN

class DXM_UTIL_DECL CCrypto{

public:
	// -----------------------------
	static dxm_uint32 Crc32( const char* buffer, dxm_uint32 size );
	static std::string Md5( const char* buffer, dxm_uint32 size );
	static std::string Sha1( const char* buffer, dxm_uint32 size );

	static dxm_uint32 XXHash( const char* buffer, dxm_uint32 size, dxm_uint32 seed );

	static std::string EncryptXXTEA(const char* buffer, dxm_uint32 size, const std::string& key);
	static std::string DecryptXXTEA(const char* buffer, dxm_uint32 size, const std::string& key);

	static std::string EncryptBase64(const char* buffer, dxm_uint32 size);
	static std::string DecryptBase64(const char* buffer, dxm_uint32 size);

	static std::string EncryptXOR(const char* buffer, dxm_uint32 size, const std::string& key);
	static std::string DecryptXOR(const char* buffer, dxm_uint32 size, const std::string& key);

	static void EncryptXORSelf(char* buffer, dxm_uint32 size, const std::string& key);
	static void DecryptXORSelf(char* buffer, dxm_uint32 size, const std::string& key);

	// -----------------------------
	static dxm_uint32 Crc32( const std::string& content );
	static std::string Md5( const std::string& content );
	static std::string Sha1( const std::string& content );

	static dxm_uint32 XXHash( const std::string& content, dxm_uint32 hash_type );

	static std::string EncryptXXTEA(const std::string& content, const std::string& key);
	static std::string DecryptXXTEA(const std::string& content, const std::string& key);

	static std::string EncryptBase64(const std::string& content);
	static std::string DecryptBase64(const std::string& content);

	static std::string EncryptXOR(const std::string& content, const std::string& key);
	static std::string DecryptXOR(const std::string& content, const std::string& key);
};

NS_UTIL_END
NS_DXM_END

#endif
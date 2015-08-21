#include "precompiled.h"
#include "util_crypto.h"

#include "dxm_crc32.hpp"
#include "dxm_md5.hpp"
#include "dxm_sha1.hpp"
#include "dxm_xxhash.h"
#include "dxm_xxtea.h"
#include "dxm_base64.h"
#include "dxm_xor.h"

#include "dxm/platform.h"

dxm_uint32 dxm::util::CCrypto::Crc32( const std::string& content )
{
	return Crc32( content.data(), content.size() );
}

dxm_uint32 dxm::util::CCrypto::Crc32( const char* buffer, dxm_uint32 size )
{
	return CRC32::Calculate( buffer, size );
}

std::string dxm::util::CCrypto::Md5( const std::string& content )
{
	return Md5( content.data(), content.size() );
}

std::string dxm::util::CCrypto::Md5( const char* buffer, dxm_uint32 size )
{
	return MD5::StringCalculate( buffer, size );
}

std::string dxm::util::CCrypto::Sha1( const std::string& content )
{
	return Sha1( content.data(), content.size() );
}

std::string dxm::util::CCrypto::Sha1( const char* buffer, dxm_uint32 size )
{
	return SHA1::StringCalculate( buffer, size );
}


std::string dxm::util::CCrypto::EncryptXXTEA( const std::string& content, const std::string& key )
{
	return EncryptXXTEA(content.c_str(), content.size(), key);
}

std::string dxm::util::CCrypto::EncryptXXTEA( const char* buffer, dxm_uint32 size, const std::string& key )
{
	dxm_uint32 ret_length = 0;
	unsigned char* ebuffer = dxm_xxtea_encrypt( (const unsigned char *)buffer, size, (const unsigned char *)key.c_str(), key.size(), &ret_length);
	std::string ret_string;
	ret_string.resize(ret_length);
	char* output_data = DXM_STRING_PTR(&ret_string);
	memcpy( output_data, ebuffer, ret_length);
	free(ebuffer);
	return ret_string;
}

std::string dxm::util::CCrypto::DecryptXXTEA( const std::string& content, const std::string& key )
{
	return DecryptXXTEA(content.c_str(), content.size(), key);
}

std::string dxm::util::CCrypto::DecryptXXTEA( const char* buffer, dxm_uint32 size, const std::string& key )
{
	dxm_uint32 ret_length = 0;
	unsigned char* dbuffer = dxm_xxtea_decrypt( (const unsigned char *)buffer, size, (const unsigned char *)key.c_str(), key.size(), &ret_length);
	std::string ret_string;
	ret_string.resize(ret_length);
	char* output_data = DXM_STRING_PTR(&ret_string);
	memcpy( output_data, dbuffer, ret_length);
	free(dbuffer);
	return ret_string;
}

std::string dxm::util::CCrypto::EncryptBase64( const std::string& content)
{
	return EncryptBase64(content.data(), content.size());
}

std::string dxm::util::CCrypto::EncryptBase64( const char* buffer, dxm_uint32 size )
{
	if(size==0){
		return "";
	}

	char* out = (char*)malloc((size*15)/3);/* Technically 4/3 of input, but take some margin */
	dxm_uint32 out_size = 0;
	dxm_uint32 sum_size = 0;
	dxm_base64_state S;
	dxm_base64_encode_init(&S);
	dxm_base64_encode_update(&S, buffer, size, out+sum_size, &out_size);
	sum_size += out_size;
	dxm_base64_encode_final(&S, out+sum_size, &out_size);
	sum_size += out_size;

	std::string ret_string;
	ret_string.resize(sum_size);
	char* output_data = DXM_STRING_PTR(&ret_string);
	memcpy( output_data, out, sum_size);
	free(out);
	return ret_string;
}

std::string dxm::util::CCrypto::DecryptBase64( const std::string& content )
{
	return DecryptBase64(content.data(), content.size());
}

std::string dxm::util::CCrypto::DecryptBase64( const char* buffer, dxm_uint32 size )
{
	if(size==0){
		return "";
	}

	char* out = (char*)malloc((size*5)/3);/* Technically 4/3 of input, but take some margin */
	dxm_uint32 out_size = 0;
	dxm_uint32 sum_size = 0;
	dxm_base64_state S;
	dxm_base64_decode_init(&S);
	dxm_base64_decode_update(&S, buffer, size, out+sum_size, &out_size);
	sum_size += out_size;
	dxm_base64_decode_final(&S, out+sum_size, &out_size);
	sum_size += out_size;

	std::string ret_string;
	ret_string.resize(sum_size);
	char* output_data = DXM_STRING_PTR(&ret_string);
	memcpy( output_data, out, sum_size);
	free(out);
	return ret_string;
}


void dxm::util::CCrypto::EncryptXORSelf(char* buffer, dxm_uint32 size, const std::string& key)
{
	dxm_xor_encrypt_self( (unsigned char*)buffer, size, (const unsigned char *)key.c_str(), key.size());
}

void dxm::util::CCrypto::DecryptXORSelf(char* buffer, dxm_uint32 size, const std::string& key)
{
	dxm_xor_decrypt_self( (unsigned char*)buffer, size, (const unsigned char *)key.c_str(), key.size());
}

std::string dxm::util::CCrypto::EncryptXOR( const std::string& content, const std::string& key )
{
	return EncryptXOR(content.c_str(), content.size(), key);
}

std::string dxm::util::CCrypto::EncryptXOR( const char* buffer, dxm_uint32 size, const std::string& key )
{
	dxm_uint32 ret_length = 0;
	unsigned char* ebuffer = dxm_xor_encrypt( (const unsigned char *)buffer, size, (const unsigned char *)key.c_str(), key.size(), &ret_length);
	if(ebuffer==0){
		return "";
	}
	std::string ret_string;
	ret_string.resize(ret_length);
	char* output_data = DXM_STRING_PTR(&ret_string);
	memcpy( output_data, ebuffer, ret_length);
	free(ebuffer);
	return ret_string;
}

std::string dxm::util::CCrypto::DecryptXOR( const std::string& content, const std::string& key )
{
	return DecryptXOR(content.c_str(), content.size(), key);
}

std::string dxm::util::CCrypto::DecryptXOR( const char* buffer, dxm_uint32 size, const std::string& key )
{
	dxm_uint32 ret_length = 0;
	unsigned char* dbuffer = dxm_xor_decrypt( (const unsigned char *)buffer, size, (const unsigned char *)key.c_str(), key.size(), &ret_length);
	if(dbuffer==0){
		return "";
	}

	std::string ret_string;
	ret_string.resize(ret_length);
	char* output_data = DXM_STRING_PTR(&ret_string);
	memcpy( output_data, dbuffer, ret_length);
	free(dbuffer);
	return ret_string;
}

dxm_uint32 dxm::util::CCrypto::XXHash( const std::string& content, dxm_uint32 seed )
{
	return DXM_XXH32(content.data(), content.size(), seed);
}

dxm_uint32 dxm::util::CCrypto::XXHash( const char* buffer, dxm_uint32 size, dxm_uint32 seed )
{
	return DXM_XXH32(buffer, size, seed);
}

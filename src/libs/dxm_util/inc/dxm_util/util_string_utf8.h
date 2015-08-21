#ifndef __DXM_UTIL_STRING_UTF8_H___
#define __DXM_UTIL_STRING_UTF8_H___

#include <string>
#include <list>

#include "dxm_util_lib.h"

NS_DXM_BEGIN
NS_UTIL_BEGIN

namespace string 
{
	class DXM_UTIL_DECL uft8{
	public:
		static std::wstring s2ws(const std::string& str);
		static std::string ws2s(const std::wstring& str);

		static dxm_boolean find(const std::string& str, const std::list<std::string>& word_list);
		static std::string replace(const std::string& str, const std::list<std::string>& word_list, const std::string& replace_word);

	private:
		// constants used in UTF-8 conversions
		static const unsigned char _lead1 = 0xC0;      //110xxxxx
		static const unsigned char _lead1_mask = 0x1F; //00011111
		static const unsigned char _lead2 = 0xE0;      //1110xxxx
		static const unsigned char _lead2_mask = 0x0F; //00001111
		static const unsigned char _lead3 = 0xF0;      //11110xxx
		static const unsigned char _lead3_mask = 0x07; //00000111
		static const unsigned char _lead4 = 0xF8;      //111110xx
		static const unsigned char _lead4_mask = 0x03; //00000011
		static const unsigned char _lead5 = 0xFC;      //1111110x
		static const unsigned char _lead5_mask = 0x01; //00000001
		static const unsigned char _cont = 0x80;       //10xxxxxx
		static const unsigned char _cont_mask = 0x3F;  //00111111

		size_t VerifyUTF8( const std::string& str );
	};
}

NS_UTIL_END
NS_DXM_END

#endif
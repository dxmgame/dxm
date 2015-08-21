#include "precompiled.h"
#include "util_string_chs.h"

#include <locale.h>

NS_DXM_BEGIN
NS_UTIL_BEGIN

std::string string::chs::ws2s( const std::wstring& ws )
{
#ifdef _WIN32
	_configthreadlocale(_ENABLE_PER_THREAD_LOCALE);
#endif
	std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
	setlocale(LC_ALL, "chs");
	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest,0,_Dsize);
	wcstombs(_Dest,_Source,_Dsize);
	std::string result = _Dest;
	delete []_Dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

std::wstring string::chs::s2ws( const std::string& s )
{
#ifdef _WIN32
	_configthreadlocale(_ENABLE_PER_THREAD_LOCALE);
#endif
	setlocale(LC_ALL, "chs"); 
	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest,_Source,_Dsize);
	std::wstring result = _Dest;
	delete []_Dest;
	setlocale(LC_ALL, "C");
	return result;
}

dxm_boolean string::chs::find( const std::string& str, const std::list<std::string>& word_list )
{
	// 转换为宽字符;
	std::wstring w_source_text = s2ws(str);

	// 查找文本;
	dxm_boolean is_found = false;
	auto iter = word_list.begin();
	while(iter != word_list.end()) {

		dxm_int32 pos = 0;
		std::wstring w_word_text = s2ws(*iter);

		pos = w_source_text.find(w_word_text, pos);
		if(pos != std::wstring::npos) {
			is_found = true;
			break;
		}
		++iter;
	}
	return is_found;
}

std::string string::chs::replace( const std::string& str, const std::list<std::string>& word_list, const std::string& replace_word )
{
	// 转换为宽字符;
	std::wstring w_source_text = s2ws(str);
	std::wstring w_replace_word_text = s2ws(replace_word);

	// 文本替换;
	dxm_boolean is_replaced = false;
	auto iter = word_list.begin();
	while(iter != word_list.end()) {

		dxm_int32 pos = 0;
		std::wstring w_word_text = s2ws(*iter);

		while(true) {

			pos = w_source_text.find(w_word_text, pos);
			if(pos == std::wstring::npos) {
				break;
			}
			w_source_text.replace(pos, w_word_text.length(), w_replace_word_text);
			pos += w_replace_word_text.length();
		}
		++iter;
	}

	// 转换为ASCII;
	return ws2s(w_source_text);
}

NS_UTIL_END
NS_DXM_END

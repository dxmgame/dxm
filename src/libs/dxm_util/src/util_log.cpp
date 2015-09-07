#include "precompiled.h"
#include "util_log.h"

NS_DXM_BEGIN
NS_UTIL_BEGIN

CLog::CLog(){
	SetLogCallBack([=](int level, const std::string& message){ printf("[%s] %s\n", GetLogLevelString(level).c_str(), message.c_str());});
}

void CLog::SetLogCallBack( const std::function<void(int, const std::string&)>& log_callback ){
	log_callback_ = log_callback;
}

std::string CLog::GetLogLevelString( int level )
{
	switch(level){
	case LOG_LEVEL_TRACE:
		return "TRACE";
	case LOG_LEVEL_DEBUG:
		return "DEBUG";
	case LOG_LEVEL_INFO:
		return "INFO ";
	case LOG_LEVEL_WARN:
		return "WARN ";
	case LOG_LEVEL_ERROR:
		return "ERROR";
	case LOG_LEVEL_FATAL:
		return "FATAL";
	}
	return "UNKWN";
}

void CLog::Log(LOG_LEVEL level, const char* format, ...)
{
	if(log_callback_){
#ifdef DEBUG
		if(level<LOG_LEVEL_INFO)
			return;
#endif // DEBUG
		va_list args;
		va_start(args, format);
		char buffer[4096];
		// MSVC 8 deprecates vsnprintf(), so we want to suppress warning
		// 4996 (deprecated function) there.
#if DXM_PLATFORM_WIN32  // We are on Windows.
#pragma warning(push)          // Saves the current warning state.
#pragma warning(disable:4996)  // Temporarily disables warning 4996.
		const int size =
			vsnprintf(buffer, sizeof(buffer)/sizeof(buffer[0]) - 1, format, args);
#pragma warning(pop)           // Restores the warning state.
#else  // We are on Linux or Mac OS.
		const int size =
			vsnprintf(buffer, sizeof(buffer)/sizeof(buffer[0]) - 1, format, args);
#endif  // GTEST_OS_WINDOWS
		va_end(args);
		log_callback_(level, buffer);
	}
}

NS_UTIL_END
NS_DXM_END
#ifndef _DXM_UTIL_LOG_H_
#define _DXM_UTIL_LOG_H_

//////////////////////////////////////////////////////////////////////////
/*
   -- init the log: DXM_LOG_EX.SetLog( your_log_function_call_back );
   -- use the log: DXM_LOG_EX( DXM_LOG_LEVEL_TRACE, "some log string" );

   -- 备注：  1. 一个应用程序或动态连接库有且仅有一个LOG回调;
             2. 回调函数调用是非多线程安全。(回调函数是会被多线程调用的，请注意回调函数内容的多线程安全。);
*/

#include <stdio.h>
#include <string>
#include <stdarg.h>
#include <functional>

#include "dxm_util_lib.h"

NS_DXM_BEGIN
NS_UTIL_BEGIN

/// \brief 通用日志模块;
/// 
/// 详细说明;
class DXM_UTIL_DECL CLog
{
public:
	enum LOG_LEVEL{
		LOG_LEVEL_TRACE,
		LOG_LEVEL_DEBUG,
		LOG_LEVEL_INFO,
		LOG_LEVEL_WARN,
		LOG_LEVEL_ERROR,
		LOG_LEVEL_FATAL,
	};

	CLog();

	void SetLogCallBack( const std::function<void(int, const std::string&)>& log_callback );

	void Log(LOG_LEVEL level, const char* format, ...);

	std::string GetLogLevelString( int level );

	DXM_SINGLETON(CLog);
private:
	std::function<void(int, const std::string&)> log_callback_;
};

NS_UTIL_END
NS_DXM_END

// 宏接口调用;
#define sLog dxm::util::CLog::GetInstancePtr()
#define sLogTrace(format, ...) sLog->Log(dxm::util::CLog::LOG_LEVEL_TRACE, format, ##__VA_ARGS__)
#define sLogDebug(format, ...) sLog->Log(dxm::util::CLog::LOG_LEVEL_DEBUG, format, ##__VA_ARGS__)
#define sLogInfo(format, ...) sLog->Log(dxm::util::CLog::LOG_LEVEL_INFO, format, ##__VA_ARGS__)
#define sLogWarn(format, ...) sLog->Log(dxm::util::CLog::LOG_LEVEL_WARN, format, ##__VA_ARGS__)
#define sLogError(format, ...) sLog->Log(dxm::util::CLog::LOG_LEVEL_ERROR, format, ##__VA_ARGS__)
#define sLogFatal(format, ...) sLog->Log(dxm::util::CLog::LOG_LEVEL_FATAL, format, ##__VA_ARGS__)
#define sLogAssert(cond, description) \
if (! (cond)) { \
	sLogError("%s(%d): %s", __FILE__, __LINE__, description);  \
}



#endif // _MISC_DXM_LOG_H_

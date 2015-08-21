#include "precompiled.h"
#include "util_time.h"


dxm_uint64 dxm::util::CTime::NowTime(void)
{
	time_t time_sec; 

#if (DXM_TARGET_PLATFORM == DXM_PLATFORM_WIN32)  
	time_sec = time(0); 
#else  
	struct timeval now;  
	gettimeofday(&now, NULL);  
	time_sec = now.tv_sec; 
#endif
	return time_sec;
}

dxm_uint64 dxm::util::CTime::ToTime(std::string date_time_string)
{
	struct tm tm; 

#if (DXM_TARGET_PLATFORM == DXM_PLATFORM_WIN32)  
	SYSTEMTIME xSys;
	memset(&xSys,0,sizeof(SYSTEMTIME));
	sscanf(date_time_string.c_str(),"%hu-%hu-%hu %hu:%hu:%hu",&xSys.wYear,&xSys.wMonth,&xSys.wDay,&xSys.wHour,&xSys.wMinute,&xSys.wSecond);
	tm.tm_year = xSys.wYear - 1900;
	tm.tm_mon = xSys.wMonth - 1;
	tm.tm_mday = xSys.wDay;
	tm.tm_hour = xSys.wHour;
	tm.tm_min = xSys.wMinute;
	tm.tm_sec = xSys.wSecond;
	tm.tm_wday = xSys.wDayOfWeek;
	tm.tm_isdst = _daylight;
#else
	strptime(date_time_string.c_str(), "%Y-%m-%d %H:%M:%S" , &tm); 
#endif // (DXM_TARGET_PLATFORM == DXM_PLATFORM_WIN32)  

	return mktime(&tm);
}

std::string dxm::util::CTime::ToTimeSpanString(dxm_int64 time_span)
{
	char buff[128] = {0};
	sprintf(buff, "%02lld:%02lld:%02lld", time_span/(60*60),(time_span/60)%60, time_span%60);
	return buff;
}

std::string dxm::util::CTime::ToDateTimeString(dxm_uint64 time)
{
	time_t time_v = time;
	struct tm *tm; 
	tm = localtime(&time_v); 
	char buff[128] = {0};
	int nSize = strftime(buff,128,"%Y-%m-%d %H:%M:%S",tm);
	return buff;
// 	int year = tm->tm_year + 1900; 
// 	int month = tm->tm_mon + 1; 
// 	int day = tm->tm_mday; 
// 	int hour= tm->tm_hour; 
// 	int min= tm->tm_min; 
// 	int second= tm->tm_sec;
//  sprintf(buff, "%04d-%02d-%02d %02d:%02d:%02d", );
}

std::string dxm::util::CTime::ToTimeString(dxm_uint64 time)
{
	time_t time_v = time;
	struct tm *tm; 
	tm = localtime(&time_v); 
	char buff[128] = {0};
	int nSize = strftime(buff,128,"%H:%M:%S",tm);
	return buff;
}

std::string dxm::util::CTime::ToDateString(dxm_uint64 time)
{
	time_t time_v = time;
	struct tm *tm; 
	tm = localtime(&time_v); 
	char buff[128] = {0};
	int nSize = strftime(buff,128,"%Y-%m-%d",tm);
	return buff;
}

dxm_int64 dxm::util::CTime::DiffTime(dxm_uint64 time1, dxm_uint64 time2)
{
	return time1 - time2;
}

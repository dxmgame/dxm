#ifndef __DXM_UTIL_TIMER_H__
#define __DXM_UTIL_TIMER_H__

#include "dxm_util_lib.h"

NS_DXM_BEGIN
NS_UTIL_BEGIN

class DXM_UTIL_DECL CTime{
public:
	static dxm_uint64 NowTime( void );
	static dxm_uint64 ToTime( std::string date_time_string );
	static dxm_int64 DiffTime( dxm_uint64 time1, dxm_uint64 time2 );

	static std::string ToTimeSpanString( dxm_int64 time_span );
	static std::string ToDateTimeString( dxm_uint64 time );
	static std::string ToTimeString( dxm_uint64 time );
	static std::string ToDateString( dxm_uint64 time );
};

NS_UTIL_END
NS_DXM_END
#endif // __DXM_TIMER_H__

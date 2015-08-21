#ifndef __DXM_UTIL_TIMER_H__
#define __DXM_UTIL_TIMER_H__

#include "dxm_util_lib.h"

NS_DXM_BEGIN
NS_UTIL_BEGIN

class DXM_UTIL_DECL CTimer{
public:
	static dxm_uint64 GetMicroSecond( void );
	static dxm_uint32 GetMilliSecond( void );
};

/**
* ����interval_ֵ������ˢ������current_����current_>=interval_��passed�ˡ�
* �˹������ڣ����ѭ�������ͣ��Զ����ñ�������ֻ��Reset����ѭ�����á�
*    1. some_timer.set_interval(40)
*    2. some_timer.update(eapsed_time)
*    3. if(some_timer.isPassed()) { DoLoopThing(); some_timer.reset()}
*/
class DXM_UTIL_DECL CIntervalTimer
{
public:
	CIntervalTimer();

	void Update( dxm_uint32 diff);
	bool IsPassed();
	void Reset();

	DXM_PROPERTY(dxm_uint32, interval);
	DXM_PROPERTY_READONLY(dxm_int32, current);
};

/** ������ʱ��;
* ����expiry_timeֵ������ˢ�¼���expiry_time����expiry_time<=0��passed�ˡ�[expiry:��ֹ������]
* �˹������ڣ�����ʱ������ͣ����ٵ�����Ҫ�������ʱ��;
*    1. some_timer.set_expiry_time(40)
*    2. some_timer.Update(eapsed_time)
*    3. if(some_timer.Passed()) { DoLoopThing(); some_timer.Reset(40)}
*/
class DXM_UTIL_DECL CExpiryTimer
{
public:
	CExpiryTimer(dxm_uint32 expiry);

	void Update(dxm_uint32 diff);
	bool IsPassed(void) const;
	void Reset(dxm_uint32 expiry_time);

	DXM_PROPERTY(dxm_uint32, expiry_time);
};

/** ��ȥ��ʱ��;
* �����ϴ�ʱ�������ȥ����ʱ��;
*/
class DXM_UTIL_DECL CElapsedTimer		{
public:

	CElapsedTimer();
	dxm_uint32	GetElapsedTime(dxm_boolean reset=true);
private:
	dxm_uint32 time_begin_;

};

NS_UTIL_END
NS_DXM_END
#endif // __DXM_TIMER_H__

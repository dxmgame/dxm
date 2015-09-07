#ifndef _UTIL_TODO_LIST_H_
#define _UTIL_TODO_LIST_H_

#include "dxm_util_lib.h"

#include <list>
#include <mutex>

#include "dxm/platform.h"

NS_DXM_BEGIN
NS_UTIL_BEGIN

/*
 *	�������б�;
 */
class DXM_UTIL_DECL CBatchTaskList : std::enable_shared_from_this<CBatchTaskList>
{
public:
	typedef std::shared_ptr<CBatchTaskList>	Ptr;

	CBatchTaskList(void);
	~CBatchTaskList(void);

	// ��������: �˲������̲߳���;
	dxm_boolean PushTask(std::function<void(CBatchTaskList::Ptr&)> task_function);

	// ִ������;
	void DoAllTask(std::function<void()> complete_callback);

	// ��������;
	dxm_int32 GetTotalTask(void);
	dxm_int32 GetUncompletedTask(void);

	// δ����������ü������첽�����ʱ���ֶ��������ü����������������������;
	void AddUncompletedTaskRef( void );
	void ReleaseUncompletedTaskRef( void );
private:

	std::list< std::function<void(CBatchTaskList::Ptr&)> > task_function_list_;
	std::function<void()> complete_callback_; 

	dxm_int32 task_uncompleted_count_;
	std::mutex task_uncompleted_count_mutex_;

	dxm_int32 task_total_;

	dxm_boolean is_doing_;
};

NS_UTIL_END
NS_DXM_END

#endif
#ifndef _UTIL_TODO_LIST_H_
#define _UTIL_TODO_LIST_H_

#include "dxm_util_lib.h"

#include <list>
#include <mutex>

#include "dxm/platform.h"

NS_DXM_BEGIN
NS_UTIL_BEGIN

/*
 *	批任务列表;
 */
class DXM_UTIL_DECL CBatchTaskList : std::enable_shared_from_this<CBatchTaskList>
{
public:
	typedef std::shared_ptr<CBatchTaskList>	Ptr;

	CBatchTaskList(void);
	~CBatchTaskList(void);

	// 推送任务: 此操作单线程操作;
	dxm_boolean PushTask(std::function<void(CBatchTaskList::Ptr&)> task_function);

	// 执行任务;
	void DoAllTask(std::function<void()> complete_callback);

	// 任务总数;
	dxm_int32 GetTotalTask(void);
	dxm_int32 GetUncompletedTask(void);

	// 未完成任务引用计数：异步任务的时候手动调用引用计数来保持任务的生命周期;
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
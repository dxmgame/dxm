#include "precompiled.h"
#include "util_batch_task_list.h"

NS_DXM_BEGIN
NS_UTIL_BEGIN

CBatchTaskList::CBatchTaskList( void )
{
	task_uncompleted_count_ = 0;
	task_total_ = 0;
	is_doing_ = false;
}

CBatchTaskList::~CBatchTaskList( void )
{

}

dxm_boolean CBatchTaskList::PushTask(std::function<void(CBatchTaskList::Ptr&)> task_function)
{
	if(is_doing_) {
		DXM_ASSERT(false, "");
		return false;
	}

	// 添加任务为单线程操作, 不用加锁;
	task_function_list_.push_back(task_function);
	++ task_uncompleted_count_;			
	++task_total_;
	return true;
}

void CBatchTaskList::DoAllTask(std::function<void()> complete_callback)
{
	if(is_doing_) {
		DXM_ASSERT(false, "");
		return;
	}

	is_doing_ = true;
	complete_callback_ = complete_callback;

	auto iter = task_function_list_.begin();
	while(iter != task_function_list_.end()) {

		auto task_function = *iter;
		if(task_function) {
			task_function(shared_from_this());
			ReleaseUncompletedTaskRef();
		}
		++iter;
	}

	task_function_list_.clear();
}

void CBatchTaskList::AddUncompletedTaskRef(void)
{
	if (!is_doing_) {
		return;
	}
	std::unique_lock<std::mutex> lock(task_uncompleted_count_mutex_);
	++task_uncompleted_count_;
}

void CBatchTaskList::ReleaseUncompletedTaskRef( void )
{
	if(!is_doing_) {
		return;
	}  
	std::unique_lock<std::mutex> lock(task_uncompleted_count_mutex_);
	--task_uncompleted_count_;	 
	if(task_uncompleted_count_ == 0) {	   
		if(complete_callback_) {	  			
			complete_callback_();
			task_total_ = 0;
			is_doing_ = false;
		}
	}
}

dxm_int32 CBatchTaskList::GetTotalTask( void )
{
	return task_total_;
}

dxm_int32 CBatchTaskList::GetUncompletedTask(void)
{
	return  task_uncompleted_count_;
}


NS_UTIL_END
NS_DXM_END
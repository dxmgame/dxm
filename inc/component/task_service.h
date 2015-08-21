#ifndef _____TASK_SERVICE_H_________
#define _____TASK_SERVICE_H_________

#include "component_service/component_service.h"
#include "log_service.h"

#include <boost/bind.hpp>

namespace Foundation{
	namespace TaskService{
		
		typedef boost::function<void ()> TaskCallBack;
		/*
		*	时间任务：可以进行延时，循环等操作的任务
		*/
		class ITimerTask
		{
		public:
			typedef boost::shared_ptr<ITimerTask> Ptr;

			ITimerTask(IComponent::Ptr task_service_depend):task_service_depend_(task_service_depend) {}
			virtual ~ITimerTask() {}

			/*
			*	投递时间任务
			*/
			virtual bool Post( const TaskCallBack& callback, int milliseconds, bool is_repeat ) = 0;

			/*
			*	重新投递当前时间任务
			*/
			virtual bool Post( int milliseconds, bool is_repeat ) = 0;

			/*
			*	取消正在进行的时间任务
			*/
			virtual void Cancel() = 0;

			/*
			*	投递任务，该函数用于直接传递函数指针
			*/
			template<class T>
			void Post( T callback, int milliseconds, bool is_repeat ){ TaskCallBack callback_temp = boost::bind(callback); Post( callback_temp, milliseconds, is_repeat); }
		protected:
			/*
			 *	潜规则：由组件创建产生给应用层使用的对象，该对象必须引用该组件。
			 */
			IComponent::Ptr task_service_depend_; 
		};

		/*
		*	串型任务：该对象投递的任务是串型的
		*/
		class IStrandTask
		{
		public:
			typedef boost::shared_ptr<IStrandTask> Ptr;

			IStrandTask(IComponent::Ptr task_service_depend):task_service_depend_(task_service_depend) {}
			virtual ~IStrandTask() {}

			/*
			*	投递任务
			*/
			virtual void Post( const TaskCallBack& callback ) = 0;

			/*
			*	创建串型时间任务
			*/
			virtual ITimerTask::Ptr CreateTimeTask() = 0;

			/*
			*	投递任务，该函数用于直接传递函数指针
			*/
			template<class T>
			void Post( T callback ){ TaskCallBack callback_temp = boost::bind(callback); Post( callback_temp ); }
		protected:
			/*
			 *	潜规则：由组件创建产生给应用层使用的对象，该对象必须引用该组件。
			 */
			IComponent::Ptr task_service_depend_; 
		};
		/*
		*	任务服务
		*/
		class ITaskService: public IComponent
		{
		public:
			typedef boost::shared_ptr<ITaskService> Ptr;

			virtual ~ITaskService() {}

			/*
			*	设置任务服务的线程数
			*/
			virtual void set_thread_count( int thread_count ) = 0;
			
			/*
			 *	设置日志对象
			 */
			virtual void set_log( Foundation::LogService::ILog::Ptr log ) = 0;

			/*
			*	投递任务
			*/
			virtual void Post( const TaskCallBack& callback ) = 0; 

			/*
			*	创建串型任务
			*/
			virtual IStrandTask::Ptr CreateStrandTask() = 0;

			/*
			*	创建时间任务
			*/
			virtual ITimerTask::Ptr CreateTimerTask() = 0;

			/*
			*	投递任务，该函数用于直接传递函数指针
			*/
			template<class T>
			void Post( T callback ){ TaskCallBack callback_temp = boost::bind(callback); Post( callback_temp ); }
		};
	}
}


#endif
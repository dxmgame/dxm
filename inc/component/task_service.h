#ifndef _____TASK_SERVICE_H_________
#define _____TASK_SERVICE_H_________

#include "component_service/component_service.h"
#include "log_service.h"

#include <boost/bind.hpp>

namespace Foundation{
	namespace TaskService{
		
		typedef boost::function<void ()> TaskCallBack;
		/*
		*	ʱ�����񣺿��Խ�����ʱ��ѭ���Ȳ���������
		*/
		class ITimerTask
		{
		public:
			typedef boost::shared_ptr<ITimerTask> Ptr;

			ITimerTask(IComponent::Ptr task_service_depend):task_service_depend_(task_service_depend) {}
			virtual ~ITimerTask() {}

			/*
			*	Ͷ��ʱ������
			*/
			virtual bool Post( const TaskCallBack& callback, int milliseconds, bool is_repeat ) = 0;

			/*
			*	����Ͷ�ݵ�ǰʱ������
			*/
			virtual bool Post( int milliseconds, bool is_repeat ) = 0;

			/*
			*	ȡ�����ڽ��е�ʱ������
			*/
			virtual void Cancel() = 0;

			/*
			*	Ͷ�����񣬸ú�������ֱ�Ӵ��ݺ���ָ��
			*/
			template<class T>
			void Post( T callback, int milliseconds, bool is_repeat ){ TaskCallBack callback_temp = boost::bind(callback); Post( callback_temp, milliseconds, is_repeat); }
		protected:
			/*
			 *	Ǳ�������������������Ӧ�ò�ʹ�õĶ��󣬸ö���������ø������
			 */
			IComponent::Ptr task_service_depend_; 
		};

		/*
		*	�������񣺸ö���Ͷ�ݵ������Ǵ��͵�
		*/
		class IStrandTask
		{
		public:
			typedef boost::shared_ptr<IStrandTask> Ptr;

			IStrandTask(IComponent::Ptr task_service_depend):task_service_depend_(task_service_depend) {}
			virtual ~IStrandTask() {}

			/*
			*	Ͷ������
			*/
			virtual void Post( const TaskCallBack& callback ) = 0;

			/*
			*	��������ʱ������
			*/
			virtual ITimerTask::Ptr CreateTimeTask() = 0;

			/*
			*	Ͷ�����񣬸ú�������ֱ�Ӵ��ݺ���ָ��
			*/
			template<class T>
			void Post( T callback ){ TaskCallBack callback_temp = boost::bind(callback); Post( callback_temp ); }
		protected:
			/*
			 *	Ǳ�������������������Ӧ�ò�ʹ�õĶ��󣬸ö���������ø������
			 */
			IComponent::Ptr task_service_depend_; 
		};
		/*
		*	�������
		*/
		class ITaskService: public IComponent
		{
		public:
			typedef boost::shared_ptr<ITaskService> Ptr;

			virtual ~ITaskService() {}

			/*
			*	�������������߳���
			*/
			virtual void set_thread_count( int thread_count ) = 0;
			
			/*
			 *	������־����
			 */
			virtual void set_log( Foundation::LogService::ILog::Ptr log ) = 0;

			/*
			*	Ͷ������
			*/
			virtual void Post( const TaskCallBack& callback ) = 0; 

			/*
			*	������������
			*/
			virtual IStrandTask::Ptr CreateStrandTask() = 0;

			/*
			*	����ʱ������
			*/
			virtual ITimerTask::Ptr CreateTimerTask() = 0;

			/*
			*	Ͷ�����񣬸ú�������ֱ�Ӵ��ݺ���ָ��
			*/
			template<class T>
			void Post( T callback ){ TaskCallBack callback_temp = boost::bind(callback); Post( callback_temp ); }
		};
	}
}


#endif
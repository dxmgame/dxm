#ifndef _____LOG_SERVICE_H_________
#define _____LOG_SERVICE_H_________

#include "component_service/component_service.h"

namespace Foundation{

	namespace LogService
	{
		enum LogLevel{
			LOG_LEVEL_TRACE,  
			LOG_LEVEL_DEBUG,  
			LOG_LEVEL_INFO,  
			LOG_LEVEL_WARN,  
			LOG_LEVEL_ERROR, 
			LOG_LEVEL_FATAL,
		};

		class ILog
		{
		public:
			typedef boost::shared_ptr<ILog> Ptr;
			ILog(IComponent::Ptr component_depend):component_depend_(component_depend) {}
			virtual ~ILog( void ) {}

			/*
			 *	��־��¼
			 */
			virtual void Output(int level, const char* msg, ...) = 0;
			virtual void Output(const char* msg, ...) = 0; // level=LOG_LEVEL_INFO
			virtual void operator() (int level, const char* msg, ...) = 0;
			virtual void operator() (const char* msg, ...) = 0;	// level=LOG_LEVEL_INFO

			/*
			 *	nested diagnostic contexts
			 */
			virtual void NDC_Push(const char* ndc, ...) = 0;
			virtual void NDC_Pop() = 0;

			/*
			 *	�����������
			 */
			virtual void Fatal(const char* szMsg, ...) = 0;
			virtual void Error(const char* szMsg, ...) = 0;
			virtual void Warn (const char* szMsg, ...) = 0;
			virtual void Info (const char* szMsg, ...) = 0;
			virtual void Debug(const char* szMsg, ...) = 0;
			virtual void Trace(const char* szMsg, ...) = 0;

		protected:
			/*
			 *	Ǳ�������������������Ӧ�ò�ʹ�õĶ��󣬸ö���������ø������
			 */
			IComponent::Ptr component_depend_; 

		};

		class ILogService:public IComponent
		{
		public:
			typedef boost::shared_ptr<ILogService> Ptr;

			virtual ~ILogService(){}

			virtual bool Initialize( const std::string& log_config ) = 0;	
			virtual void Shutdown( void ) = 0;	
			virtual ILog::Ptr CreateLog( const std::string& log_name ) = 0;
			
			/*
			 *	nested diagnostic contexts
			 */
			virtual void NDC_Push(const char* ndc, ...) = 0;
			virtual void NDC_Pop() = 0;
		};
	}
}


#endif

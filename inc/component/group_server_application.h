#ifndef ___GROUP_SERVER_APPLICATION_H______
#define ___GROUP_SERVER_APPLICATION_H______

#include "component_service/component_service.h"

namespace Foundation{

	class IGroupServerApplication:public IComponent
	{
	public:
		typedef boost::shared_ptr<IGroupServerApplication> Ptr;
		virtual ~IGroupServerApplication() {}

		// 初始化GroupServer间的依赖以及会话的注册(GroupServer初始化前);
		virtual bool OnInitialize( const std::string& xml_file ) = 0;
		
		// 框架初始化事件(GroupServer初始化后);
		virtual bool OnGroupServerStart() = 0;

		// 框架关闭事件;
		virtual void OnShutdown() = 0;
	};
}


#endif
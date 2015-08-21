#ifndef ___GROUP_SERVER_APPLICATION_H______
#define ___GROUP_SERVER_APPLICATION_H______

#include "component_service/component_service.h"

namespace Foundation{

	class IGroupServerApplication:public IComponent
	{
	public:
		typedef boost::shared_ptr<IGroupServerApplication> Ptr;
		virtual ~IGroupServerApplication() {}

		// ��ʼ��GroupServer��������Լ��Ự��ע��(GroupServer��ʼ��ǰ);
		virtual bool OnInitialize( const std::string& xml_file ) = 0;
		
		// ��ܳ�ʼ���¼�(GroupServer��ʼ����);
		virtual bool OnGroupServerStart() = 0;

		// ��ܹر��¼�;
		virtual void OnShutdown() = 0;
	};
}


#endif
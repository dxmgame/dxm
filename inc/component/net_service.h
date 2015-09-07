#ifndef _____NET_SERVICE_H_________
#define _____NET_SERVICE_H_________

#include "dxm_com/dxm_com.h"
#include "log_service.h"

namespace Foundation{

	namespace NetService{

		class IConnectorSink;

		/*
		 *	��INetService��������
		 */
		class IConnector: public boost::enable_shared_from_this<IConnector>
		{
		public:
			typedef boost::shared_ptr<IConnector> Ptr;
			typedef boost::weak_ptr<IConnector> WeakPtr;

			/*
			 *	Ǳ�������������������Ӧ�ò�ʹ�õĶ��󣬸ö���������ø��������ֹ����ͷ�����DLL�ͷź���øö�������ı�������.(��̳к�����һ�����)
			 */
			IConnector(IComponent::Ptr component_depend):component_depend_(component_depend) {}
			
			virtual ~IConnector() {}

			/*
			 *	����ԭʼ����
			 */
			virtual bool Send( const void * buffer, unsigned int size ) = 0;
			
			/*
			 *	�ر�����
			 */
			virtual void Shutdown( void ) = 0;

			/*
			 *	����������Ϣ
			 */
			virtual std::string local_ip() = 0;
			virtual int local_port() = 0;
			virtual std::string remote_ip() = 0;
			virtual int remote_port() = 0;

		protected:
			/*
			 *	Ǳ�������������������Ӧ�ò�ʹ�õĶ��󣬸ö���������ø��������ֹ����ͷ�����DLL�ͷź���øö�������ı�������.
			 */
			IComponent::Ptr component_depend_; 
		};

		/*
		 *	��INetService��������
		 */
		class IServer: public boost::enable_shared_from_this<IServer>
		{
		public:
			typedef boost::shared_ptr<IServer> Ptr;

			/*
			 *	Ǳ�������������������Ӧ�ò�ʹ�õĶ��󣬸ö���������ø��������ֹ����ͷ�����DLL�ͷź���øö�������ı�������.(��̳к�����һ�����)
			 */
			IServer(IComponent::Ptr component_depend):component_depend_(component_depend) {}
			virtual ~IServer() {}
			
			/*
			 *	�رշ�����
			 */
			virtual void Shutdown( void ) = 0;

			/*
			 *	������Ϣ
			 */
			virtual std::string local_ip() = 0;
			virtual int local_port() = 0;

		protected:
			/*
			 *	Ǳ�������������������Ӧ�ò�ʹ�õĶ��󣬸ö���������ø��������ֹ����ͷ�����DLL�ͷź���øö�������ı�������.
			 */
			IComponent::Ptr component_depend_; 
		};

		class INetService : public IComponent
		{
		public:
			typedef boost::shared_ptr<INetService> Ptr;
			typedef boost::weak_ptr<INetService> WeakPtr;
			virtual ~INetService(void)	{}

			/*
			 *	��ʼ��������
			 *		mdz_net_service  Initialize ����������libevent���绷��
			 *		mdz_net_service  Shutdown �����ر����еķ����������е�����
			 */
			virtual bool Initialize( Foundation::LogService::ILog::Ptr log, unsigned int recv_size ) = 0;	
			virtual bool Shutdown( void ) = 0;	

			/*
			 *	����˼����������ӽ���ʱ����AcceptCallback
			 */
			virtual IServer::Ptr Listen( const char * local_ip, int local_port, IConnectorSink* sink ) = 0;
			
			/*
			 *	�ͻ������ӣ����ӳɹ�ʱ����AcceptCallback
			 */
			virtual IConnector::Ptr	Connect( const char * remote_ip, int remote_port, IConnectorSink* sink ) = 0;
		};

		// �¼�������
		class IConnectorSink
		{
		public:
			virtual void OnConnect( const IConnector::Ptr& ) = 0;
			virtual void OnRecv( const IConnector::Ptr&, const void* buffer, unsigned int size ) = 0;
			virtual void OnDisconnect( const IConnector::Ptr& ) = 0;

			virtual IConnectorSink* Clone() = 0;
			virtual void Release() = 0;
		};
	}
}


#endif

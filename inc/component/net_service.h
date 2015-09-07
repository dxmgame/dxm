#ifndef _____NET_SERVICE_H_________
#define _____NET_SERVICE_H_________

#include "dxm_com/dxm_com.h"
#include "log_service.h"

namespace Foundation{

	namespace NetService{

		class IConnectorSink;

		/*
		 *	由INetService创建产生
		 */
		class IConnector: public boost::enable_shared_from_this<IConnector>
		{
		public:
			typedef boost::shared_ptr<IConnector> Ptr;
			typedef boost::weak_ptr<IConnector> WeakPtr;

			/*
			 *	潜规则：由组件创建产生给应用层使用的对象，该对象必须引用该组件，防止组件释放引起DLL释放后调用该对象引起的崩溃问题.(请继承后填入一个组件)
			 */
			IConnector(IComponent::Ptr component_depend):component_depend_(component_depend) {}
			
			virtual ~IConnector() {}

			/*
			 *	发送原始数据
			 */
			virtual bool Send( const void * buffer, unsigned int size ) = 0;
			
			/*
			 *	关闭连接
			 */
			virtual void Shutdown( void ) = 0;

			/*
			 *	基本网络信息
			 */
			virtual std::string local_ip() = 0;
			virtual int local_port() = 0;
			virtual std::string remote_ip() = 0;
			virtual int remote_port() = 0;

		protected:
			/*
			 *	潜规则：由组件创建产生给应用层使用的对象，该对象必须引用该组件，防止组件释放引起DLL释放后调用该对象引起的崩溃问题.
			 */
			IComponent::Ptr component_depend_; 
		};

		/*
		 *	由INetService创建产生
		 */
		class IServer: public boost::enable_shared_from_this<IServer>
		{
		public:
			typedef boost::shared_ptr<IServer> Ptr;

			/*
			 *	潜规则：由组件创建产生给应用层使用的对象，该对象必须引用该组件，防止组件释放引起DLL释放后调用该对象引起的崩溃问题.(请继承后填入一个组件)
			 */
			IServer(IComponent::Ptr component_depend):component_depend_(component_depend) {}
			virtual ~IServer() {}
			
			/*
			 *	关闭服务器
			 */
			virtual void Shutdown( void ) = 0;

			/*
			 *	基本信息
			 */
			virtual std::string local_ip() = 0;
			virtual int local_port() = 0;

		protected:
			/*
			 *	潜规则：由组件创建产生给应用层使用的对象，该对象必须引用该组件，防止组件释放引起DLL释放后调用该对象引起的崩溃问题.
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
			 *	初始化与销毁
			 *		mdz_net_service  Initialize 在做基本的libevent网络环境
			 *		mdz_net_service  Shutdown 在做关闭所有的服务器和所有的连接
			 */
			virtual bool Initialize( Foundation::LogService::ILog::Ptr log, unsigned int recv_size ) = 0;	
			virtual bool Shutdown( void ) = 0;	

			/*
			 *	服务端监听，有连接进来时调用AcceptCallback
			 */
			virtual IServer::Ptr Listen( const char * local_ip, int local_port, IConnectorSink* sink ) = 0;
			
			/*
			 *	客户端连接，连接成功时调用AcceptCallback
			 */
			virtual IConnector::Ptr	Connect( const char * remote_ip, int remote_port, IConnectorSink* sink ) = 0;
		};

		// 事件接收器
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

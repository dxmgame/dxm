#ifndef __NETWORK_CONTEXT_H___
#define __NETWORK_CONTEXT_H___


#include "dxm/platform.h"

NS_XE_BEGIN 
class IConnectorSink;

struct EndPoint{
	std::string ip;
	xe_uint32 port;
};
/*
 *	由INetWorkContext创建产生
 */
class IConnector: public boost::enable_shared_from_this<IConnector>
{
public:
	typedef boost::shared_ptr<IConnector> Ptr;
	typedef boost::weak_ptr<IConnector> WeakPtr;

	IConnector(IComponent::Ptr component_depend):component_depend_(component_depend) {} // 潜规则：由组件创建产生给应用层使用的对象，该对象必须引用该组件，防止组件释放引起DLL释放后调用该对象引起的崩溃问题.(请继承后填入一个组件)
	virtual ~IConnector() {}
	
	// 发起连接
	virtual bool Connect( const string& remote_ip, xe_uint32 remote_port, IConnectorSink* sink )
	// 发送原始数据
	virtual bool Send( const void * buffer, unsigned int size ) = 0; 
	// 关闭连接
	virtual void Shutdown( void ) = 0; 
	// 基本网络信息
	virtual const EndPoint& local() = 0;
	virtual const EndPoint& remote() = 0;
	virtual IConnectorSink* sink() = 0;
protected:
	IComponent::Ptr component_depend_; // 潜规则：由组件创建产生给应用层使用的对象，该对象必须引用该组件，防止组件释放引起DLL释放后调用该对象引起的崩溃问题.
};

// 事件接收器
class IConnectorSink
{
public:
	virtual void OnConnect( const IConnector::Ptr& ) = 0;
	virtual void OnRecv( const IConnector::Ptr&, void* buffer, xe_uint32 size ) = 0;
	virtual void OnDisconnect( const IConnector::Ptr& ) = 0;
	
	virtual IConnectorSink* Clone() = 0;
	virtual void Release() = 0;
};

/*
*	由INetWorkContext创建产生
*/
class IAcceptor: public boost::enable_shared_from_this<IServer>
{
public:
	typedef boost::shared_ptr<IServer> Ptr;

	IAcceptor(IComponent::Ptr component_depend):component_depend_(component_depend) {} // 潜规则：由组件创建产生给应用层使用的对象，该对象必须引用该组件，防止组件释放引起DLL释放后调用该对象引起的崩溃问题.(请继承后填入一个组件)
	virtual ~IAcceptor() {}
	// 监听
	virtual void Listen( const string& local_ip, xe_uint32 local_port, IConnectorSink* sink )
	// 关闭服务器
	virtual void Shutdown( void ) = 0; 
	// 基本信息
	virtual const EndPoint& local() = 0;
protected:
	IComponent::Ptr component_depend_; //潜规则：由组件创建产生给应用层使用的对象，该对象必须引用该组件，防止组件释放引起DLL释放后调用该对象引起的崩溃问题.
};

class INetworkContext : public IComponent
{
public:
	typedef boost::shared_ptr<INetworkContext> Ptr;
	typedef boost::weak_ptr<INetworkContext> WeakPtr;

	virtual ~INetworkContext(void)	{}

	// 
	virtual void SetLog( Foundation::LogService::ILog::Ptr log );
	virtual void SetUnitRecvSize( xe_uint32 unit_recv_size );
	virtual bool Initialize( void ) = 0;	
	virtual bool Shutdown( void ) = 0;	

	virtual IConnector::Ptr MakeConnector() = 0;
	virtual IAcceptor::Ptr MakeAcceptor() = 0;
};
NS_XE_END

#endif
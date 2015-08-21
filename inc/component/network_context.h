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
 *	��INetWorkContext��������
 */
class IConnector: public boost::enable_shared_from_this<IConnector>
{
public:
	typedef boost::shared_ptr<IConnector> Ptr;
	typedef boost::weak_ptr<IConnector> WeakPtr;

	IConnector(IComponent::Ptr component_depend):component_depend_(component_depend) {} // Ǳ�������������������Ӧ�ò�ʹ�õĶ��󣬸ö���������ø��������ֹ����ͷ�����DLL�ͷź���øö�������ı�������.(��̳к�����һ�����)
	virtual ~IConnector() {}
	
	// ��������
	virtual bool Connect( const string& remote_ip, xe_uint32 remote_port, IConnectorSink* sink )
	// ����ԭʼ����
	virtual bool Send( const void * buffer, unsigned int size ) = 0; 
	// �ر�����
	virtual void Shutdown( void ) = 0; 
	// ����������Ϣ
	virtual const EndPoint& local() = 0;
	virtual const EndPoint& remote() = 0;
	virtual IConnectorSink* sink() = 0;
protected:
	IComponent::Ptr component_depend_; // Ǳ�������������������Ӧ�ò�ʹ�õĶ��󣬸ö���������ø��������ֹ����ͷ�����DLL�ͷź���øö�������ı�������.
};

// �¼�������
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
*	��INetWorkContext��������
*/
class IAcceptor: public boost::enable_shared_from_this<IServer>
{
public:
	typedef boost::shared_ptr<IServer> Ptr;

	IAcceptor(IComponent::Ptr component_depend):component_depend_(component_depend) {} // Ǳ�������������������Ӧ�ò�ʹ�õĶ��󣬸ö���������ø��������ֹ����ͷ�����DLL�ͷź���øö�������ı�������.(��̳к�����һ�����)
	virtual ~IAcceptor() {}
	// ����
	virtual void Listen( const string& local_ip, xe_uint32 local_port, IConnectorSink* sink )
	// �رշ�����
	virtual void Shutdown( void ) = 0; 
	// ������Ϣ
	virtual const EndPoint& local() = 0;
protected:
	IComponent::Ptr component_depend_; //Ǳ�������������������Ӧ�ò�ʹ�õĶ��󣬸ö���������ø��������ֹ����ͷ�����DLL�ͷź���øö�������ı�������.
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
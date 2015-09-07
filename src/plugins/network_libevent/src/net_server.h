#ifndef NET_ENGINE_ACCEPTOR_H
#define NET_ENGINE_ACCEPTOR_H

class CNetService;
class CServer:public IServer
{
public:
	typedef boost::shared_ptr<CServer> Ptr;

	CServer( IComponent::Ptr component_depend, CNetService* net_service_impl );
	~CServer(void);

	bool Init(const char * ip, unsigned int port, IConnectorSink* sink );

	inline evconnlistener* evconn_listener() { return evconn_listener_; }

	/*
	*	关闭服务器
	*/
	virtual void Shutdown( void );

	/*
	*	基本信息
	*/
	virtual std::string local_ip();
	virtual int local_port();

	void ShutdownReal();

	inline bool will_to_shutdown() { return will_to_shutdown_; }

	inline CNetService* net_service_impl() { return net_service_impl_; }

	inline IConnectorSink* sink_clone() { return sink_?sink_->Clone():0; }
private:
	evconnlistener * evconn_listener_;

	std::string local_ip_;
	int local_port_;

	IConnectorSink* sink_;
	
	CNetService* net_service_impl_;

	/*
	 *	网络服务关闭订阅者;
	 */
	xenon::util_boost::CEventListener net_shutdown_listener_;
	bool will_to_shutdown_;
};

#endif

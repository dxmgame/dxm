#ifndef NET_ENGINE_SAFE_BUFFEREVENT_H
#define NET_ENGINE_SAFE_BUFFEREVENT_H

class CNetService;
class CConnector : public IConnector
{
public:
	typedef boost::shared_ptr<CConnector> Ptr;

	CConnector(IComponent::Ptr component_depend, CNetService* net_service_impl);
	~CConnector(void);


	virtual void set_sink(IConnectorSink* sink);

	/*
	*	发送数据;
	*/
	virtual bool Send( const void * buffer, unsigned int size );

	/*
	*	关闭连接;
	*/
	virtual void Shutdown( void );

	/*
	*	基本信息;
	*/
	virtual std::string local_ip() { return local_ip_; }
	virtual int local_port() { return local_port_; }
	virtual std::string remote_ip() { return remote_ip_; }
	virtual int remote_port() { return remote_port_; }


	bool Init(int sock_id, IConnectorSink* sink, struct sockaddr* remote_addr);
	void ShutDownImmediately(void);

	void OnSend(unsigned int size);
	void OnRecv(const void * buffer, unsigned int size);

	inline bool shutdown(void) { return shutdown_; }
	inline int sock_id() { return sock_id_;}

	void set_close_connect(xenon::util_boost::CEventListener& close_connetion) { net_shutdown_listener_ = close_connetion; }

	std::string* GetRecvBuffer(void);
private:
	// bufferevent * bufferev_;
	boost::shared_ptr<bufferevent> bufferev_;
	boost::weak_ptr<bufferevent> bufferev_weak_;

	bool shutdown_;
	void * data_;
	int sock_id_;

	IConnectorSink* sink_;

	/*
	 *	网络服务关闭订阅者;
	 */
	xenon::util_boost::CEventListener net_shutdown_listener_;
	CNetService* net_service_impl_;
	
	std::string local_ip_;
	std::string remote_ip_;
	int	local_port_;
	int remote_port_;
};

#endif

#include "precompiled.h"

#include "net_server.h"
#include "net_service_internal.h"
#include "net_connector.h"

#define ACCEPTOR_LISTEN_BACKLOG 5

//	listener event
void OnAccepted(struct evconnlistener* linstener, evutil_socket_t acceptSocket, struct sockaddr* addr, int sockLen, void* ptr) {
	CServer* server = (CServer*)ptr;
	
	if(server->will_to_shutdown()) {
#ifdef DXM_PLATFORM_WIN32
		closesocket(acceptSocket);
#else
		close(acceptSocket);
#endif // DXM_PLATFORM_WIN32
		server->ShutdownReal();
	}
	else {
		CConnector::Ptr ptr(new CConnector(server->net_service_impl()->shared_from_this(), server->net_service_impl()));
		ptr->Init(acceptSocket, server->sink_clone(), addr);
	}
}

void OnAcceptError(struct evconnlistener* linstener, void *ptr)
{
	CServer * server = (CServer*)ptr;
	DXM_COMPONENT_LOG(DXM_COMPONENT_LOG_LEVEL_ERROR, "OnAcceptError Appear.");
//	server->ShutdownReal();
}

CServer::CServer(IComponent::Ptr component_depend, CNetService* net_service_impl):IServer(component_depend),net_service_impl_(net_service_impl)
{
	evconn_listener_ = 0;
	will_to_shutdown_ = false;
}

CServer::~CServer( void ) {
	evconn_listener_ = 0;
	net_service_impl_ = 0;
	if(sink_)
	{
		sink_->Release();
		sink_ = 0;
	}
}

bool CServer::Init( const char * ip, unsigned int port, IConnectorSink* sink ) {
	
	//	request socket
	DXM_NDC("CServer::Init");

	net_shutdown_listener_ = net_service_impl_->RegisterShutdownEventListener(boost::bind(&IServer::Shutdown, shared_from_this()));
	sink_ = sink;

	//	config address
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(port);
	evconn_listener_ = evconnlistener_new_bind(
		net_service_impl_->ev_base(), 
		OnAccepted, 
		this, 
		LEV_OPT_THREADSAFE | LEV_OPT_REUSEABLE, 
		ACCEPTOR_LISTEN_BACKLOG, 
		(sockaddr*)&sin, 
		sizeof(sockaddr_in));

	if(evconn_listener_ == 0) {
		net_shutdown_listener_.Disconnect();
		DXM_COMPONENT_LOG(DXM_COMPONENT_LOG_LEVEL_ERROR, "server(%s:%d) evconnlistener_new_bind failed.", ip, port);
		return false;
	}

	evconnlistener_set_error_cb(evconn_listener_, OnAcceptError);
	local_ip_ = ip;
	local_port_ = port;
	DXM_COMPONENT_LOG(DXM_COMPONENT_LOG_LEVEL_INFO, "server(%s:%d) start", local_ip_.c_str(), local_port_);
	return true;
}

void CServer::Shutdown( void )
{
	if(will_to_shutdown_==false){
		// 设置will_to_shutdown_标志位, 创建空连接;
		will_to_shutdown_ = true;
		net_service_impl_->NullConnect(local_ip_.c_str(), local_port_);
	}
}

std::string CServer::local_ip()
{
	return local_ip_;
}

int CServer::local_port()
{
	return local_port_;
}

void CServer::ShutdownReal()
{
	IServer::Ptr ref = shared_from_this();
	DXM_NDC("CServer::ShutdownReal");
	DXM_COMPONENT_LOG(DXM_COMPONENT_LOG_LEVEL_INFO, "server(%s:%d) shutdown", local_ip_.c_str(), local_port_);
	evconnlistener_free(evconn_listener_);
	net_shutdown_listener_.Disconnect();
}

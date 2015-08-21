#ifndef _____SESSION_H_________
#define _____SESSION_H_________

#include "boost/signals2.hpp"

#include "dxm_util_boost/util_boost_tls_buffer.h"
#include "dxm_util_boost/util_boost_buffer.h"
#include "dxm_util_boost/util_boost_serializer.h"
#include "dxm_util_boost/util_boost_event_set.hpp"
#include "dxm_util_component/util_component_log.h"

#include "component_service/component_service.h"
#include "component/net_service.h"
#include "component/task_service.h"
#include "protocol/dxm_protocol_header.h"


class ISessionCore;
namespace Foundation{
 
	/*
	 *  Ŀ�ģ�;
	 *		1. ���ISessionע��������ַ�ʽ�ӳ�������֮���������д;
	 *		2. ���ճ������Լ������ܽ��ܵķ�ʽ�����IPacker���ⲿʹ��;
	 *	ISession �����̳� NetSessionHelper::ISessionRTTI;
	 *	IPacker ʵ�ֺ�������NetSessionʹ�ã������ⲿ����;
	 */ 
	namespace NetSession{

		typedef boost::function<bool(const void * , unsigned int ) > DelegateSend;
		typedef boost::function<void(const xenon::util_boost::CBuffer&  ) > DelegateRecv;
		typedef boost::function<void((bool))> DelegateShutdown;
		typedef boost::function<bool(void)> DelegateIsValid;
		typedef boost::function<bool(boost::shared_ptr<ISessionCore>&)> DelegateAcceptSession;
		typedef boost::function<void(const std::string&)> DelegateSetKey;

		typedef boost::function<bool(boost::shared_ptr<ISessionCore>&)> CallbackAcceptSession;

		typedef boost::function<void(())> CallbackConnect;
		typedef boost::function<void(const xenon::util_boost::CBuffer& logic_packet)> CallbackProcess;
		typedef boost::function<void()> CallbackDisconnect;

		class ISession:public IComponent
		{
		public:
			typedef boost::shared_ptr<ISession> Ptr;
			typedef boost::weak_ptr<ISession> WeakPtr;

			virtual ~ISession() {}

			/*
			 *	�¼��ص������� ���� �Ͽ� ���ջỰ;
			 */
			virtual CallbackConnect _callback_connect() = 0;
			virtual CallbackProcess _callback_process() = 0;
			virtual CallbackDisconnect _callback_disconnect() = 0;

			/*
			 *	�й����ֲ��������� ���� �ر� �ж� �����������Ƿ�����;
			 */
			virtual void  _RegisterSendDelegate( const DelegateSend& send_callback ) = 0;
			virtual void  _RegisterRecvDelegate( const DelegateRecv& recv_callback ) = 0;
			virtual void  _RegisterShutdownDelegate( const DelegateShutdown& shutdown_callback ) = 0;
			virtual void  _RegisterIsValidDelegate( const DelegateIsValid& is_valid_callback ) = 0;
			virtual void  _RegisterSetKeyDelegate( const DelegateSetKey& set_key_callback ) = 0;

			/*
			 *	����SessionCore��������Session�ܹ�����Session;
			 */
			virtual CallbackAcceptSession _callback_accept_session() = 0;
			virtual void  _RegisterAcceptDelegate( const CallbackAcceptSession& accept_session_callback ) = 0;

			struct SessionInfo{
				std::string local_ip;
				std::string remote_ip;
				int	local_port;
				int remote_port;
			};

			virtual SessionInfo& session_info() = 0;
			virtual void set_session_info(const SessionInfo& session_info) = 0;

			virtual bool Send(const void * buffer, unsigned int size) = 0;
			virtual void Recv(const xenon::util_boost::CBuffer& logic_packet) = 0;
			virtual void Shutdown( bool shut_down_root_ ) = 0;
			virtual bool IsValid() = 0;
			
			virtual bool Send( const xenon::util_boost::CBuffer& logic_packet ) = 0;
			virtual bool Send( int msg_code, const xenon::util_boost::CBuffer& buffer ) = 0;
			virtual bool Send( int msg_code, const void * buffer, unsigned int size ) = 0;
			virtual void Recv( const void * buffer, unsigned int size) = 0;
			virtual void Recv( int msg_code, const xenon::util_boost::CBuffer& buffer ) = 0;
			virtual void Recv( int msg_code, const void * buffer, unsigned int size ) = 0;

			virtual void SetKey( const std::string& key) = 0;

			template<class MessageType>
			bool Send( int msg_code, const MessageType& message ){
				std::string buffer;
				message.SerializeToString(&buffer);
				return Send( msg_code, buffer.data(), buffer.size() );
			}
		};

		class IPacker:public IComponent
		{
		public:
			typedef boost::shared_ptr<IPacker> Ptr;

			typedef boost::function<void( const xenon::util_boost::CBuffer&  logic_packet )> UnpackRecvCallback;
			typedef boost::function<bool( const void * , unsigned int)> PackSendCallback;

			virtual ~IPacker() {}
			virtual bool Pack( const void * pack_buffer, unsigned int pack_size, const PackSendCallback& send_callback) = 0;
			virtual bool Unpack( const void * buffer, unsigned int size, const UnpackRecvCallback& recv ) = 0;
			virtual void SetKey( const std::string& key ) = 0;
		};

		class ISessionServer
		{
		public:
			typedef boost::shared_ptr<ISessionServer> Ptr;

			ISessionServer(IComponent::Ptr component_depend):component_depend_(component_depend) {}
			virtual ~ISessionServer() {}

			virtual  void Close() = 0;
			virtual  bool IsValid() = 0;
		
		protected:
			/*
			 *	Ǳ�������������������Ӧ�ò�ʹ�õĶ��󣬸ö���������ø����;
			 */
			IComponent::Ptr component_depend_; 
		};

		class ISessionService:public IComponent{
		public:
			typedef boost::shared_ptr<ISessionService> Ptr;

			virtual ~ISessionService() {}

			virtual void Initialize( Foundation::TaskService::ITaskService::Ptr task_service, Foundation::NetService::INetService::Ptr net_service ) = 0;
			virtual void Shutdown() = 0;

			// �Ự�������;
			virtual ISessionServer::Ptr SessionListen( const char * local_ip, int local_port, const ComponentID& session_component_id, const ComponentID& packer_component_id="dxm_packer" ) = 0;
			// �Ự����(ָ���ڲ������Ự����);
			virtual ISession::Ptr SessionConnect( const char * remote_ip, int remote_port, const ComponentID& session_component_id, const ComponentID& packer_component_id="dxm_packer", Foundation::TaskService::IStrandTask::Ptr rector_strand = Foundation::TaskService::IStrandTask::Ptr() ) = 0;
			virtual ISession::Ptr SessionConnect( ISession::Ptr parent_session, const ComponentID& session_component_id, Foundation::TaskService::IStrandTask::Ptr rector_strand = Foundation::TaskService::IStrandTask::Ptr() ) = 0;
			// �Ự����(ָ���Ự����);
			virtual bool SessionConnect( const char * remote_ip, int remote_port, const ISession::Ptr& session, const ComponentID& packer_component_id="dxm_packer", Foundation::TaskService::IStrandTask::Ptr rector_strand = Foundation::TaskService::IStrandTask::Ptr() ) = 0;
			virtual bool SessionConnect( ISession::Ptr parent_session, const ISession::Ptr& session, Foundation::TaskService::IStrandTask::Ptr rector_strand = Foundation::TaskService::IStrandTask::Ptr() ) = 0;
		};
	}

	namespace NetSessionHelper{

		class CSessionHelper
		{
		public:
			static char* GetPacketBuffer(unsigned int size) {
				static xenon::util_boost::CTlsBuffer tls_buffer(8096);
				return tls_buffer.GetBuffer(size);
			}
		};

		template< class T >
		class ISessionRTTI:public NetSession::ISession
		{
		public:
			typedef boost::shared_ptr<T> Ptr;

			ISessionRTTI(){
				set_callback_connect(boost::bind(&ISessionRTTI<T>::ConnectCallback, this));
				set_callback_process(boost::bind(&ISessionRTTI<T>::ProcessCallback, this, _1));
				set_callback_disconnect(boost::bind(&ISessionRTTI<T>::DisconnectCallback, this));
				
				set_callback_accept_session(boost::bind(&ISessionRTTI<T>::_AcceptSession, this, _1));
			}

			virtual ~ISessionRTTI() {}

			void ConnectCallback() { self_ = IComponent::QueryInterface<T>(); OnConnect();  }
			void ProcessCallback(const xenon::util_boost::CBuffer& logic_packet) { OnProcess(logic_packet); }
			void DisconnectCallback() { OnDisconnect(); self_.reset();}

			/*
			 *	�ⲿ���Լ̳и��麯����Ҳ���� set_callback_connect set_callback_process set_callback_disconnect �����Լ��ĺ���;
			 */
			virtual void OnConnect() {}
			virtual void OnProcess(const xenon::util_boost::CBuffer& logic_packet) {}
			virtual void OnDisconnect() {}

			/*
			 *	Session��Ự���ӵ���;
			 */
			virtual NetSession::CallbackConnect _callback_connect() { return callback_connect_; }
			virtual NetSession::CallbackProcess _callback_process() { return callback_process_; }
			virtual NetSession::CallbackDisconnect _callback_disconnect() { return callback_disconnect_; }
			virtual NetSession::CallbackAcceptSession _callback_accept_session() { return callback_accept_session_; }

			void  set_callback_connect(const NetSession::CallbackConnect& callback_connect) { callback_connect_ = callback_connect;}
			void  set_callback_process(const NetSession::CallbackProcess& callback_process) { callback_process_ = callback_process; }
			void  set_callback_disconnect(const NetSession::CallbackDisconnect& callback_disconnect) { callback_disconnect_ = callback_disconnect;}
			void  set_callback_accept_session(const NetSession::CallbackAcceptSession& callback_accept_session) { callback_accept_session_ = callback_accept_session;}

			/*
			 *	Session���»Ự���ӵ���;
			 */
			virtual bool _AcceptSession( boost::shared_ptr<ISessionCore>& session_core)
			{
				return delegate_accept_session_(session_core);
			}

			virtual void  _RegisterSendDelegate( const NetSession::DelegateSend& send_callback ){
				delegate_send_ = send_callback;
			}
			virtual void  _RegisterRecvDelegate( const NetSession::DelegateRecv& recv_callback ){
				delegate_recv_ = recv_callback;
			}
			virtual void  _RegisterShutdownDelegate( const NetSession::DelegateShutdown& shutdown_callback ){
				delegate_shutdown_ = shutdown_callback;
			}
			virtual void  _RegisterIsValidDelegate( const NetSession::DelegateIsValid& is_valid_callback ){
				delegate_is_valid_ = is_valid_callback;
			}
			virtual void  _RegisterAcceptDelegate( const NetSession::DelegateAcceptSession& accept_session_callback ){
				delegate_accept_session_ = accept_session_callback;
			}

			virtual void _RegisterSetKeyDelegate( const NetSession::DelegateSetKey& set_key_callback ){
				delegate_set_key_ = set_key_callback;
			}

			bool Send(const void * buffer, unsigned int size) { 
				if(delegate_send_){
					return delegate_send_(buffer,size); 
				}
				return false;
			}
			void Recv(const xenon::util_boost::CBuffer& logic_packet)
			{
				if(delegate_recv_){
					delegate_recv_(logic_packet);
				}
			}
			void Shutdown( bool shut_down_root_ ) { 
				if(delegate_shutdown_){
					delegate_shutdown_(shut_down_root_); 
				}
			}
			bool IsValid() {
				if(delegate_is_valid_){
					return delegate_is_valid_();
				}
				return false;
			}
			virtual void SetKey( const std::string& key){
				if(delegate_set_key_){
					return delegate_set_key_(key);
				}
			}

			bool Send( const xenon::util_boost::CBuffer& logic_packet )
			{
				return Send(logic_packet.ptr(), logic_packet.size());
			}

			bool Send( int msg_code, const xenon::util_boost::CBuffer& buffer )
			{
				// �Ż�����;
				unsigned int size = buffer.size();
				char* buff = CSessionHelper::GetPacketBuffer(size+4);
				*((unsigned int*)buff)=msg_code;
				memcpy(buff+4, buffer.ptr(), size);
				XE_COMPONENT_LOG(XE_COMPONENT_LOG_LEVEL_TRACE, "send data [msgcode:%d]", msg_code );
				return Send( buff, size+4 );
			}

			bool Send( int msg_code, const void * buffer, unsigned int size )
			{
				char* buff = CSessionHelper::GetPacketBuffer(size+4);
				*((unsigned int*)buff)=msg_code;
				if(buffer && size) memcpy(buff+4, buffer, size);
				XE_COMPONENT_LOG(XE_COMPONENT_LOG_LEVEL_TRACE, "send data [msgcode:%d]", msg_code );
				return Send( buff, size+4 );
			}

			void Recv(const void * buffer, unsigned int size) 
			{
				void* buff_alloc = malloc(size);
				memcpy(buff_alloc, buffer, size);
				xenon::util_boost::CBuffer buffer_alloc(buff_alloc, size, xenon::util::CBufferDelecter::DeleteStruct);
				Recv(buffer_alloc); 
			}

			void Recv( int msg_code, const xenon::util_boost::CBuffer& buffer )
			{
				unsigned int size = buffer.size();
				char* buff = CSessionHelper::GetPacketBuffer(size+4);
				*((unsigned int*)buff)=msg_code;
				memcpy(buff+4, buffer.ptr(), size);
				XE_COMPONENT_LOG(XE_COMPONENT_LOG_LEVEL_TRACE, "recv data [msgcode:%d]", msg_code );
				return Recv( buff, size+4 );
			}

			void Recv( int msg_code, const void * buffer, unsigned int size )
			{
				char* buff = CSessionHelper::GetPacketBuffer(size+4);
				*((unsigned int*)buff)=msg_code;
				if(buffer && size) memcpy(buff+4, buffer, size);
				XE_COMPONENT_LOG(XE_COMPONENT_LOG_LEVEL_TRACE, "recv data [msgcode:%d]", msg_code );
				return Recv( buff, size+4 );
			}

			NetSession::ISession::SessionInfo& session_info() { return session_info_; }
			void set_session_info(const NetSession::ISession::SessionInfo& session_info) { session_info_ = session_info; }

			// �趨Component��RTTI�ܹ�ʶ��T;
			_COMPONENT_HEADER_BEGIN
				__COMPONENT_HEADER_INHERIT(ISession)
				if( interfaceID == typeid(T) )return static_cast<T*>(this);
			_COMPONENT_HEADER_END

			Ptr self() {
				return self_;
			}

		private:
			NetSession::ISession::SessionInfo session_info_;

			/// \brief ����йܸ�session_service�ĺ���;
			NetSession::DelegateSend delegate_send_;
			NetSession::DelegateRecv delegate_recv_;
			NetSession::DelegateShutdown delegate_shutdown_;
			NetSession::DelegateIsValid delegate_is_valid_;
			NetSession::DelegateAcceptSession delegate_accept_session_;
			NetSession::DelegateSetKey delegate_set_key_;
			

			/// \brief �ĸ��ڲ��ӳ��Ļص�����;
			NetSession::CallbackConnect callback_connect_;
			NetSession::CallbackProcess callback_process_;
			NetSession::CallbackDisconnect callback_disconnect_;
			NetSession::CallbackAcceptSession callback_accept_session_;

			Ptr self_;
		};


		template<class T>
		class CLogicSession:public Foundation::NetSessionHelper::ISessionRTTI<T>
		{
		public:
			typedef void (T::*MsgHandle)(const xenon::util_boost::CBuffer&);
			typedef boost::shared_ptr<T> Ptr;
			typedef boost::weak_ptr<T> WeakPtr;

			struct LogicSessionHandles
			{
				MsgCode msg_code;
				MsgHandle msg_handle;
			};

			CLogicSession() {}
			virtual ~CLogicSession() {}


			virtual void OnConnect(){

			}
			virtual void OnDisconnect(){

			}
			/**	======================================*/

			virtual void OnProcess(const xenon::util_boost::CBuffer& logic_packet){

				MsgCode msg_code;
				xenon::util_boost::CSerializer logic_packet_stream(logic_packet, logic_packet.size());
				logic_packet_stream>>msg_code;

				MsgHandle handle = _GetMsgHandle(msg_code);
				if(handle){
					T* child = (T*)this;
					xenon::util_boost::CBuffer buff = logic_packet_stream.data_last_read();
					(child->*(handle))(buff);
				}
				else if(msg_handle_map_[COMMAND_DEFAULT].msg_handle)
				{
					T* child = (T*)this;
					(child->*(msg_handle_map_[COMMAND_DEFAULT].msg_handle))(logic_packet);

				}
			}

		protected:
			static MsgHandle _GetMsgHandle(MsgCode msg_code){

				for(int i=0; msg_handle_map_[i].msg_code!=COMMAND_END; i++)
				{
					if(msg_handle_map_[i].msg_code == msg_code)
						return msg_handle_map_[i].msg_handle;
				}
				return 0;
			}

			static LogicSessionHandles msg_handle_map_[];
		};
		
#define BEGIN_LOGIC_MESSAGE_MAP(LogicSessionType, default_msg_handle) \
	template<> Foundation::NetSessionHelper::CLogicSession<LogicSessionType>::LogicSessionHandles Foundation::NetSessionHelper::CLogicSession<LogicSessionType>::msg_handle_map_[]={\
		{COMMAND_DEFAULT,  default_msg_handle},\

#define ON_LOGIC_MESSAGE(msg_code, msg_handle) {msg_code,  msg_handle},

#define END_LOGIC_MESSAGE_MAP() {COMMAND_END, 0}};
	}
}


#endif

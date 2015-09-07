#ifndef __DXM_UTIL_EVENT_SET_HPP___
#define __DXM_UTIL_EVENT_SET_HPP___

#include "dxm_util_lib.h"

#include <map>
#include <mutex>

NS_DXM_BEGIN
NS_UTIL_BEGIN

#define MAX_EVENT_ALLOC_ID 0xFFFFFFFF

/// \brief ������;
/// 
/// ��ϸ˵��;
template <class Signature>
class _CFunctionMap{

public:
	struct MapItem{
		MapItem(dxm_uint32 a_key, std::function<Signature> a_object) { key=a_key; object=a_object; next_ = 0;}
		dxm_uint32 key;
		MapItem* next_;
		std::function<Signature> object;
	};

	_CFunctionMap(){
		head_ = 0;
		alloc_id_ = 0;
		size_ = 0; 
	}
	~_CFunctionMap(){

		clear();
	}

	dxm_uint32 size() {
		return size_;
	}

	std::function<Signature>* find(dxm_uint32 key){

		MapItem* item = head_;
		while(item){
			if(item->key == key){
				return item->object;
			}
			item = item->next_;
		}
		return 0;
	}
	template<class Function>
	dxm_uint32 add(const Function& obj){
		
		if(alloc_id_ == MAX_EVENT_ALLOC_ID){
			alloc_id_ = 0;
		}
		dxm_uint32 id = ++ alloc_id_;
		MapItem* item = new MapItem(id, obj);
		item->next_ = head_;
		head_ = item;
		++ size_;
		return id;
	}
	dxm_boolean remove(dxm_uint32 key){
		MapItem* item = head_;
		MapItem* pre_item = 0;
		while(item){
			if(item->key == key){
				if(pre_item){
					pre_item->next_ = item->next_;					
				}
				else{
					head_ = item->next_;
				}
				delete item;
				-- size_;
				return true;
			}
			pre_item = item;
			item = item->next_;
		}
		return false;
	}

	template<class Function>
	dxm_boolean remove(const Function& object){
		MapItem* item = head_;
		MapItem* pre_item = 0;
		while(item){
			if(item->object == object){
				if(pre_item){
					pre_item->next_ = item->next_;					
				}
				else{
					head_ = item->next_;
				}
				delete item;
				-- size_;
				return true;
			}
			pre_item = item;
			item = item->next_;
		}
		return false;
	}
	void clear(){
		MapItem* item = head_;
		while(item){
			MapItem* next_item = item->next_;
			delete item;
			item = next_item;
		}
		head_ = 0;
		size_ = 0;
		alloc_id_ = 0;
	}
	
	std::map<dxm_uint32, std::function<Signature> > get_map(void){
		std::map<dxm_uint32, std::function<Signature> > map;
		MapItem* item = head_;
		while(item){
			map[item->key] = item->object;
			item = item->next_;
		}
		return map;
	}

private:
	MapItem*  head_;
	dxm_uint32 size_;
	dxm_uint32 alloc_id_;
};



/// \brief �����¼�;
/// 
/// ��ϸ˵���������¼���ֻ֧��ʹ�ñ��ע������;;

class DXM_UTIL_DECL _IEvent: public std::enable_shared_from_this<_IEvent>
{
public:
	virtual void Disconnect(dxm_uint32 id) = 0;
};

/// \brief �¼��������;
/// 
/// ��ϸ˵��;

class DXM_UTIL_DECL CEventListener
{
public:
	CEventListener(void) {
		id_ = 0;
	}
	CEventListener(dxm_uint32 id, std::weak_ptr<_IEvent> event) {
		id_ = id;
		event_ = event;
	}
	~CEventListener(void) {}

	// ��������;
	CEventListener(const CEventListener& event_listener) {
		id_ = event_listener.id_;
		event_ = event_listener.event_;
	}

	// ��ֵ����;
	void operator=(const CEventListener& event_listener) {
		id_ = event_listener.id_;
		event_ = event_listener.event_;
	}

	// ע������;
	void Disconnect(void) {

		// ��Ч���;
		if(id_ == 0) {
			return;
		}

		// ��ȡ�¼�������ע��;
		std::shared_ptr<_IEvent> event = event_.lock();
		if(event) {
			event->Disconnect(id_);
		}
		event_.reset();
	}

private:
	dxm_uint32 id_; // �������;
	std::weak_ptr<_IEvent> event_; // �����¼�;
};

/// \brief ����;
/// 
/// ��ϸ˵��;

class DXM_UTIL_DECL CDummyMutex
{
public:
	void lock(void) {}
	bool try_lock(void) { return true; }
	void unlock(void) {}
};

/// \brief �¼�ʵ��;
/// 
/// ��ϸ˵��;

template<typename Signature, class Mutex = CDummyMutex>
class CEventImpl : public _IEvent
{
public:
	CEventImpl(void) {
		// alloc_id_ = 0;
	}
	~CEventImpl(void) {
	}

	// �����¼�;
	template<class Function> 
	CEventListener Connect(const Function& callback) { 

		dxm_uint32 id = _Connect(callback);
		if(id != 0) {
			return CEventListener(id, std::weak_ptr<_IEvent>(shared_from_this()));
		}
		return CEventListener(); 
	}

	// ��������;
	void Disconnect(dxm_uint32 id) {

		_Disconnect(id);
	}

	// ��������;
	template<class Function>
	void Disconnect(const Function& callback) {

		_Disconnect(callback);
	}

	// �������м���;
	void DisconnectAll(void) {

		_DisconnectAll();
	}

	// �����¼�;
	void operator()(void) { 

		std::map<dxm_uint32, std::function<Signature> > callback_map = _GetCallbackMap();

		// ���������¼�;
		auto iter = callback_map.begin();
		while(iter != callback_map.end()) {

			std::function<Signature> callback = iter->second;
			if(callback) {
				callback();
			}
			++iter;
		}
	}

	template<class T1>
	void operator()(T1 t1) { 

		std::map<dxm_uint32, std::function<Signature> > callback_map = _GetCallbackMap();

		// ���������¼�;
		auto iter = callback_map.begin();
		while(iter != callback_map.end()) {

			std::function<Signature> callback = iter->second;
			if(callback) {
				callback(t1);
			}
			++iter;
		}
	}

	template<class T1, class T2>
	void operator()(T1 t1, T2 t2) { 

		std::map<dxm_uint32, std::function<Signature> > callback_map = _GetCallbackMap();

		// ���������¼�;
		auto iter = callback_map.begin();
		while(iter != callback_map.end()) {

			std::function<Signature> callback = iter->second;
			if(callback) {
				callback(t1, t2);
			}
			++iter;
		}
	}

	template<class T1, class T2, class T3>
	void operator()(T1 t1, T2 t2, T3 t3) { 

		std::map<dxm_uint32, std::function<Signature> > callback_map = _GetCallbackMap();

		// ���������¼�;
		auto iter = callback_map.begin();
		while(iter != callback_map.end()) {

			std::function<Signature> callback = iter->second;
			if(callback) {
				callback(t1, t2, t3);
			}
			++iter;
		}
	}

	template<class T1, class T2, class T3, class T4>
	void operator()(T1 t1, T2 t2, T3 t3, T4 t4) { 

		std::map<dxm_uint32, std::function<Signature> > callback_map = _GetCallbackMap();

		// ���������¼�;
		auto iter = callback_map.begin();
		while(iter != callback_map.end()) {

			std::function<Signature> callback = iter->second;
			if(callback) {
				callback(t1, t2, t3, t4);
			}
			++iter;
		}
	}

	template<class T1, class T2, class T3, class T4, class T5>
	void operator()(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5) { 

		std::map<dxm_uint32, std::function<Signature> > callback_map = _GetCallbackMap();

		// ���������¼�;
		auto iter = callback_map.begin();
		while(iter != callback_map.end()) {

			std::function<Signature> callback = iter->second;
			if(callback) {
				callback(t1, t2, t3, t4, t5);
			}
			++iter;
		}
	}

	template<class T1, class T2, class T3, class T4, class T5, class T6>
	void operator()(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6) { 

		std::map<dxm_uint32, std::function<Signature> > callback_map = _GetCallbackMap();

		// ���������¼�;
		auto iter = callback_map.begin();
		while(iter != callback_map.end()) {

			std::function<Signature> callback = iter->second;
			if(callback) {
				callback(t1, t2, t3, t4, t5, t6);
			}
			++iter;
		}
	}

	template<class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	void operator()(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7) { 

		std::map<dxm_uint32, std::function<Signature> > callback_map = _GetCallbackMap();

		// ���������¼�;
		auto iter = callback_map.begin();
		while(iter != callback_map.end()) {

			std::function<Signature> callback = iter->second;
			if(callback) {
				callback(t1, t2, t3, t4, t5, t6, t7);
			}
			++iter;
		}
	}

	template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	void operator()(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8) { 

		std::map<dxm_uint32, std::function<Signature> > callback_map = _GetCallbackMap();

		// ���������¼�;
		auto iter = callback_map.begin();
		while(iter != callback_map.end()) {

			std::function<Signature> callback = iter->second;
			if(callback) {
				callback(t1, t2, t3, t4, t5, t6, t7, t8);
			}
			++iter;
		}
	}

	template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
	void operator()(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9) { 

		std::map<dxm_uint32, std::function<Signature> > callback_map = _GetCallbackMap();

		// ���������¼�;
		auto iter = callback_map.begin();
		while(iter != callback_map.end()) {

			std::function<Signature> callback = iter->second;
			if(callback) {
				callback(t1, t2, t3, t4, t5, t6, t7, t8, t9);
			}
			++iter;
		}
	}

	template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
	void operator()(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10) { 

		std::map<dxm_uint32, std::function<Signature> > callback_map = _GetCallbackMap();

		// ���������¼�;
		auto iter = callback_map.begin();
		while(iter != callback_map.end()) {

			std::function<Signature> callback = iter->second;
			if(callback) {
				callback(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);
			}
			++iter;
		}
	}

	dxm_int32 NumListeners(){
		std::unique_lock<Mutex> lock(mutex_);
		return callback_map_.size();
	}

protected:

	// �������;
	template<class Function>
	dxm_uint32 _Connect(const Function& callback) {

		std::unique_lock<Mutex> lock(mutex_);
		return callback_map_.add(callback);
	}

	// �Ƴ�����;
	void _Disconnect(dxm_uint32 id) {

		std::unique_lock<Mutex> lock(mutex_);
		callback_map_.remove(id);
	}

	// �Ƴ�����;
	template<class Function>
	void _Disconnect(const Function& callback) {

		std::unique_lock<Mutex> lock(mutex_);
		callback_map_.remove(callback);
	}

	// �Ƴ����м���;
	void _DisconnectAll(void) {

		std::unique_lock<Mutex> lock(mutex_);
		callback_map_.clear();
	}


	// ��ȡ������;
	std::map<dxm_uint32, std::function<Signature> > _GetCallbackMap(void) {

		std::unique_lock<Mutex> lock(mutex_);

		// ����һ���¼�������;
		return callback_map_.get_map();
	}
	Mutex mutex_; // ������;
	_CFunctionMap< Signature > callback_map_;
};

/// \brief �¼�����;
/// 
/// ��ϸ˵��;

template<typename Signature, class EventImpl>
class CEventBase
{
public:
	CEventBase(void) : impl_(new EventImpl) {}
	~CEventBase(void) {}

	// �����¼�;
	template<class Function>
	CEventListener Connect(const Function& callback) { 
		return impl_->Connect(callback);
	}

	// ��������;
	void Disconnect(dxm_uint32 id) {
		impl_->Disconnect(id);
	}

	// ��������;
	template<class Function>
	void Disconnect(Function callback) {
		impl_->Disconnect(callback);
	}

	// �������м���;
	void DisconnectAll(void) {
		impl_->DisconnectAll();
	}

	dxm_int32 NumListeners(){
		return impl_->NumListeners();
	}

	// ����������;
	template<class Function>
	void operator+=(const Function& callback) {
		Connect(callback);
	}

	template<class Function>
	void operator-=(const Function& callback) {
		Disconnect(callback);
	}

	// �����¼�;
	void operator()(void) { 
		(*impl_)();
	}

	template<class T1>
	void operator()(T1 t1) { 
		(*impl_)(t1);
	}

	template<class T1, class T2>
	void operator()(T1 t1, T2 t2) { 
		(*impl_)(t1, t2);
	}

	template<class T1, class T2, class T3>
	void operator()(T1 t1, T2 t2, T3 t3) { 
		(*impl_)(t1, t2, t3);
	}

	template<class T1, class T2, class T3, class T4>
	void operator()(T1 t1, T2 t2, T3 t3, T4 t4) { 
		(*impl_)(t1, t2, t3, t4);
	}

	template<class T1, class T2, class T3, class T4, class T5>
	void operator()(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5) { 
		(*impl_)(t1, t2, t3, t4, t5);
	}

	template<class T1, class T2, class T3, class T4, class T5, class T6>
	void operator()(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6) { 
		(*impl_)(t1, t2, t3, t4, t5, t6);
	}

	template<class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	void operator()(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7) { 
		(*impl_)(t1, t2, t3, t4, t5, t6, t7);
	}

	template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	void operator()(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8) { 
		(*impl_)(t1, t2, t3, t4, t5, t6, t7, t8);
	}

	template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
	void operator()(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9) { 
		(*impl_)(t1, t2, t3, t4, t5, t6, t7, t8, t9);
	}

	template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
	void operator()(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10) { 
		(*impl_)(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);
	}

protected:
	std::shared_ptr<EventImpl> impl_; // ʵ����;
};

/// \brief �¼�����;
/// 
/// ��ϸ˵��;

template<typename Signature>
class CEvent : public CEventBase<Signature, CEventImpl<Signature> >
{
public:
};

template<typename Signature>
class CEvent2 : public CEventBase<Signature, CEventImpl<Signature, std::mutex> >
{
public:
};

NS_UTIL_END
NS_DXM_END

#endif // __XENON_EVENT_SET_HPP___
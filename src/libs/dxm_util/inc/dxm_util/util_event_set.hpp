#ifndef __DXM_UTIL_EVENT_SET_HPP___
#define __DXM_UTIL_EVENT_SET_HPP___

#include "dxm_util_lib.h"

#include <map>
#include <mutex>

NS_DXM_BEGIN
NS_UTIL_BEGIN

#define MAX_EVENT_ALLOC_ID 0xFFFFFFFF

/// \brief 函数表;
/// 
/// 详细说明;
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



/// \brief 抽象事件;
/// 
/// 详细说明：抽象事件，只支持使用编号注销监听;;

class DXM_UTIL_DECL _IEvent: public std::enable_shared_from_this<_IEvent>
{
public:
	virtual void Disconnect(dxm_uint32 id) = 0;
};

/// \brief 事件监听句柄;
/// 
/// 详细说明;

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

	// 拷贝构造;
	CEventListener(const CEventListener& event_listener) {
		id_ = event_listener.id_;
		event_ = event_listener.event_;
	}

	// 赋值操作;
	void operator=(const CEventListener& event_listener) {
		id_ = event_listener.id_;
		event_ = event_listener.event_;
	}

	// 注销监听;
	void Disconnect(void) {

		// 无效句柄;
		if(id_ == 0) {
			return;
		}

		// 获取事件并进行注销;
		std::shared_ptr<_IEvent> event = event_.lock();
		if(event) {
			event->Disconnect(id_);
		}
		event_.reset();
	}

private:
	dxm_uint32 id_; // 监听句柄;
	std::weak_ptr<_IEvent> event_; // 所属事件;
};

/// \brief 无锁;
/// 
/// 详细说明;

class DXM_UTIL_DECL CDummyMutex
{
public:
	void lock(void) {}
	bool try_lock(void) { return true; }
	void unlock(void) {}
};

/// \brief 事件实现;
/// 
/// 详细说明;

template<typename Signature, class Mutex = CDummyMutex>
class CEventImpl : public _IEvent
{
public:
	CEventImpl(void) {
		// alloc_id_ = 0;
	}
	~CEventImpl(void) {
	}

	// 监听事件;
	template<class Function> 
	CEventListener Connect(const Function& callback) { 

		dxm_uint32 id = _Connect(callback);
		if(id != 0) {
			return CEventListener(id, std::weak_ptr<_IEvent>(shared_from_this()));
		}
		return CEventListener(); 
	}

	// 撤销监听;
	void Disconnect(dxm_uint32 id) {

		_Disconnect(id);
	}

	// 撤销监听;
	template<class Function>
	void Disconnect(const Function& callback) {

		_Disconnect(callback);
	}

	// 撤销所有监听;
	void DisconnectAll(void) {

		_DisconnectAll();
	}

	// 运行事件;
	void operator()(void) { 

		std::map<dxm_uint32, std::function<Signature> > callback_map = _GetCallbackMap();

		// 遍历运行事件;
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

		// 遍历运行事件;
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

		// 遍历运行事件;
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

		// 遍历运行事件;
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

		// 遍历运行事件;
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

		// 遍历运行事件;
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

		// 遍历运行事件;
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

		// 遍历运行事件;
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

		// 遍历运行事件;
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

		// 遍历运行事件;
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

		// 遍历运行事件;
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

	// 插入监听;
	template<class Function>
	dxm_uint32 _Connect(const Function& callback) {

		std::unique_lock<Mutex> lock(mutex_);
		return callback_map_.add(callback);
	}

	// 移除监听;
	void _Disconnect(dxm_uint32 id) {

		std::unique_lock<Mutex> lock(mutex_);
		callback_map_.remove(id);
	}

	// 移除监听;
	template<class Function>
	void _Disconnect(const Function& callback) {

		std::unique_lock<Mutex> lock(mutex_);
		callback_map_.remove(callback);
	}

	// 移除所有监听;
	void _DisconnectAll(void) {

		std::unique_lock<Mutex> lock(mutex_);
		callback_map_.clear();
	}


	// 获取监听表单;
	std::map<dxm_uint32, std::function<Signature> > _GetCallbackMap(void) {

		std::unique_lock<Mutex> lock(mutex_);

		// 拷贝一份事件监听表单;
		return callback_map_.get_map();
	}
	Mutex mutex_; // 数据锁;
	_CFunctionMap< Signature > callback_map_;
};

/// \brief 事件载体;
/// 
/// 详细说明;

template<typename Signature, class EventImpl>
class CEventBase
{
public:
	CEventBase(void) : impl_(new EventImpl) {}
	~CEventBase(void) {}

	// 监听事件;
	template<class Function>
	CEventListener Connect(const Function& callback) { 
		return impl_->Connect(callback);
	}

	// 撤销监听;
	void Disconnect(dxm_uint32 id) {
		impl_->Disconnect(id);
	}

	// 撤销监听;
	template<class Function>
	void Disconnect(Function callback) {
		impl_->Disconnect(callback);
	}

	// 撤销所有监听;
	void DisconnectAll(void) {
		impl_->DisconnectAll();
	}

	dxm_int32 NumListeners(){
		return impl_->NumListeners();
	}

	// 操作符重载;
	template<class Function>
	void operator+=(const Function& callback) {
		Connect(callback);
	}

	template<class Function>
	void operator-=(const Function& callback) {
		Disconnect(callback);
	}

	// 运行事件;
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
	std::shared_ptr<EventImpl> impl_; // 实现体;
};

/// \brief 事件载体;
/// 
/// 详细说明;

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
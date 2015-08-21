#ifndef __DXM_COM_CONTEXT_H__
#define __DXM_COM_CONTEXT_H__

#include "dxm_com_lib.h"
#include "dxm_com_interface.h"
#include "dxm_com_factory.hpp"
#include "dxm_util/util_log.h"

class DXM_COM_DECL CComContext{
public:
	CComContext();
	~CComContext();

	void SetLog(xenon::util::CLog::FuncLogCallBack callback);

	void LoadPlugin( const std::string& plugin_name );
	void LoadPlugin( const std::string& plugin_name, const std::string& plugin_path );

	/*
	 *	添加与删除组件工厂;
	 */
	void AddFactory( const ComID& component_id, const IComponent::Factory& factory );
	void AddFactory( const ComID& component_id, const IComponent::Factory& factory, boost::shared_ptr<CDynamicLib>& dynamic_lib_ptr);
	void RemoveFactory( const ComID& component_id );
	
	/*
	 *	创建组件;
	 */
	IComponent::Ptr Create( const ComID& component_id );

	void Enable( const ComID& component_id );
	void Disable( const ComID& component_id );
	bool IsEnable( const ComID& component_id );

	static CComContext* Instance();

private:
	CComponentFactories* factories_;
	CDynamicPlugins* plugins_;

public:
	/*
	 *	添加新组件，参数：组件名+组件类;
	 */
	template <class T>
	void AddFactory( const ComID& component_id ){
		AddFactory(component_id, boost::factory< T * >());
	}
	template <class T>
	void AddFactory( const ComID& component_id, boost::shared_ptr<CDynamicLib>& dynamic_lib_ptr ){
		AddFactory(component_id, boost::factory< T * >(), dynamic_lib_ptr);
	}
	/*
	 *	添加新组件，参数：组件类(默认组件名为typeid(组件类));
	 */
	template <class T>
	void AddFactory( boost::shared_ptr<CDynamicLib>& dynamic_lib_ptr ){
		ComID id = typeid(T).name();
		AddFactory(id, boost::factory< T * >(), dynamic_lib_ptr);
	}
	template <class T>
	void AddFactory( ){
		ComID id = typeid(T).name();
		AddFactory(id, boost::factory< T * >());
	}

	template <class T>
	void RemoveFactory( ){
		ComID id = typeid(T).name();
		RemoveFactory(id);
	}

	template<class InterfaceType>
	inline boost::shared_ptr<InterfaceType> Create( const ComID& component_id )
	{
		IComponent::Ptr ptr = Create(component_id);
		if(ptr)
			return ptr->QueryInterface<InterfaceType>();
		return boost::shared_ptr<InterfaceType>();
	}

	template<class InterfaceType>
	inline boost::shared_ptr<InterfaceType> Create()
	{
		ComID component_id = typeid(InterfaceType).name();
		IComponent::Ptr ptr = Create(component_id);
		if(ptr)
			return ptr->QueryInterface<InterfaceType>();
		return boost::shared_ptr<InterfaceType>();
	}


	/*  2011-12-28
	 *	新添加Create函数，保留老的函数以提供兼容性
	 *  原来boost::shared_ptr<InterfaceType> ptr = sComponentRepository->Create<xxx>(idxxx)
	 *  修改后 boost::shared_ptr<InterfaceType> ptr; sComponentRepository->Create(idxxxx, ptr); 
	 */
	template<class InterfaceType>
	inline void Create( const ComID& component_id, boost::shared_ptr<InterfaceType>& out_ptr)
	{
		IComponent::Ptr ptr = Create(component_id);
		if(ptr)
			out_ptr = ptr->QueryInterface<InterfaceType>();
		out_ptr = boost::shared_ptr<InterfaceType>();
	}

	template<class InterfaceType>
	inline void Create( boost::shared_ptr<InterfaceType>& out_ptr)
	{
		ComID component_id = typeid(InterfaceType).name();
		IComponent::Ptr ptr = Create(component_id);
		if(ptr)
			out_ptr = ptr->QueryInterface<InterfaceType>();
		out_ptr = boost::shared_ptr<InterfaceType>();
	}
	/************************************************************************/

	template <class T>
	void Enable( ){
		ComID id = typeid(T).name();
		Enable(id);
	}
	template <class T>
	void Disable( ){
		ComID id = typeid(T).name();
		Disable(id);
	}
	template <class T>
	void IsEnable( ){
		ComID id = typeid(T).name();
		IsEnable(id);
	}
};

#endif // __DXM_COM_CONTEXT_H__

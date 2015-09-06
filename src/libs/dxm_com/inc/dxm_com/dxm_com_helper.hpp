#ifndef __DXM_COM_HELPER_H___
#define __DXM_COM_HELPER_H___

#include "dxm_com_lib.h"
#include "dxm_com_factory.hpp"

NS_DXM_BEGIN

class DXM_COM_DECL CComHelper{
public:
	CComHelper();
	~CComHelper();

	void SetLog(const std::function<void(int, const std::string&)>& log_callback);

public:
	template <class T>
	void AddFactory(const ComID& component_id){
		AddFactory(component_id, detail::CComFactory::CreateFactory<T>());
	}
	template <class T>
	void AddFactory(const ComID& component_id, std::shared_ptr<CDynamicLib>& dynamic_lib_ptr){
		AddFactory(component_id, detail::CComFactory::CreateFactory<T>(), dynamic_lib_ptr);
	}
	template <class T>
	void AddFactory(boost::shared_ptr<CDynamicLib>& dynamic_lib_ptr){
		ComID id = typeid(T).name();
		AddFactory(id, detail::CComFactory::CreateFactory<T>(), dynamic_lib_ptr);
	}
	template <class T>
	void AddFactory(){
		ComID id = typeid(T).name();
		AddFactory(id, detail::CComFactory::CreateFactory<T>());
	}

	template<class InterfaceType>
	inline std::shared_ptr<InterfaceType> Create(const ComID& component_id)
	{
		ICom::Ptr ptr = Create(component_id);
		if (ptr)
			return ptr->QueryInterface<InterfaceType>();
		return std::shared_ptr<InterfaceType>();
	}

	template<class InterfaceType>
	inline std::shared_ptr<InterfaceType> Create()
	{
		ComID component_id = typeid(InterfaceType).name();
		ICom::Ptr ptr = Create(component_id);
		if (ptr)
			return ptr->QueryInterface<InterfaceType>();
		return std::shared_ptr<InterfaceType>();
	}


	/*  2011-12-28
	*	新添加Create函数，保留老的函数以提供兼容性
	*  原来boost::shared_ptr<InterfaceType> ptr = sComponentRepository->Create<xxx>(idxxx)
	*  修改后 boost::shared_ptr<InterfaceType> ptr; sComponentRepository->Create(idxxxx, ptr);
	*/
	template<class InterfaceType>
	inline void Create(const ComID& component_id, std::shared_ptr<InterfaceType>& out_ptr)
	{
		ICom::Ptr ptr = Create(component_id);
		if (ptr)
			out_ptr = ptr->QueryInterface<InterfaceType>();
		out_ptr = std::shared_ptr<InterfaceType>();
	}

	template<class InterfaceType>
	inline void Create(std::shared_ptr<InterfaceType>& out_ptr)
	{
		ComID component_id = typeid(InterfaceType).name();
		ICom::Ptr ptr = Create(component_id);
		if (ptr)
			out_ptr = ptr->QueryInterface<InterfaceType>();
		out_ptr = std::shared_ptr<InterfaceType>();
	}
};

NS_DXM_END


#endif // __DXM_COM_HELPER_H___

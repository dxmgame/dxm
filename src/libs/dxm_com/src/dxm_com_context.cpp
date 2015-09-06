#include "precompiled.h"
#include "dxm_com_context.h"
#include "dxm_factory_manager.h"
#include "dynamic_plugins.h"
#include "dxm_util/util_log.h"

NS_DXM_BEGIN
namespace detail{

	CComContext::CComContext() :factories_(new dxm::CComFactoryManager), plugins_(new CDynamicPlugins)
	{

	}

	CComContext::~CComContext()
	{
		DXM_SAFE_DELETE(factories_);
		DXM_SAFE_DELETE(plugins_);
	}

	void CComContext::SetLog(const std::function<void(int, const std::string&)>& log_callback)
	{
		sLog->SetLogCallBack(log_callback);
	}

	void CComContext::LoadPlugin(const std::string & plugin_name)
	{
		if (plugins_)
			plugins_->LoadPlugin(plugin_name);
	}

	void CComContext::AddFactory(const ComID& component_id, const detail::CComFactory::Ptr& factory)
	{
		if (factories_)
			factories_->AddFactory(component_id, factory);
	}

	void CComContext::AddFactory(const ComID& component_id, const detail::CComFactory::Ptr& factory, CDynamicLib::Ptr& dynamic_lib_ptr)
	{
		if (factories_)
			factories_->AddFactory(component_id, factory, dynamic_lib_ptr);
	}


	ICom::Ptr CComContext::CreateCom(const ComID& component_id)
	{
		if (factories_)
			return factories_->CreateCom(component_id);
		return ICom::Ptr();
	}
}

NS_DXM_END

#include "precompiled.h"
#include "dxm_com_context.h"
#include "component_plugins.h"
#include "component_factories.h"

CComContext::CComContext():factories_(new CComponentFactories), plugins_(new CComponentPlugins)
{

}

CComContext::~CComContext()
{
	XE_SAFE_DELETE(factories_);
	XE_SAFE_DELETE(plugins_);
}

void CComContext::SetLog( xenon::util::CLog::FuncLogCallBack callback )
{
	XE_LOG_EX.SetLogCallBack(callback);
}

void CComContext::LoadPlugin( const std::string & plugin_name )
{
	if(plugins_)
		plugins_->LoadPlugin(plugin_name);
}

void CComContext::LoadPlugin( const std::string& plugin_name, const std::string& plugin_path )
{
	if(plugins_)
		plugins_->LoadPlugin(plugin_name, plugin_path);
}

void CComContext::AddFactory( const ComponentID& component_id, const IComponent::Factory& factory )
{
	if(factories_)
		factories_->AddFactory( component_id, factory );
}

void CComContext::AddFactory( const ComponentID& component_id, const IComponent::Factory& factory, CDynamicLib::Ptr& dynamic_lib_ptr )
{
	if(factories_)
		factories_->AddFactory( component_id, factory, dynamic_lib_ptr );
}

void CComContext::RemoveFactory( const ComponentID& component_id )
{
	if(factories_)
		factories_->RemoveFactory( component_id );
}

IComponent::Ptr CComContext::Create( const ComponentID& component_id )
{
	if(factories_)
		return factories_->Create( component_id );
	return IComponent::Ptr();
}

void CComContext::Enable( const ComponentID& component_id )
{
	if(factories_)
		factories_->Enable( component_id );
}

void CComContext::Disable( const ComponentID& component_id )
{
	if(factories_)
		factories_->Disable( component_id );
}

bool CComContext::IsEnable( const ComponentID& component_id )
{
	if(factories_)
		return factories_->IsEnable( component_id );
	return false;
}

CComContext* CComContext::Instance()
{
	static CComContext self;
	return &self;
}



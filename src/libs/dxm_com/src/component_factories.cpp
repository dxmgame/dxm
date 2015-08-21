#include "precompiled.h"
#include "component_factories.h"
#include "component_plugins.h"

CComponentFactories::CComponentFactories()
{
	XE_LOG_EX(xenon::util::CLog::LOG_LEVEL_INFO, "[component:info ] CComponentFactories::CComponentFactories plugins start");
}

CComponentFactories::~CComponentFactories()
{
	XE_LOG_EX(xenon::util::CLog::LOG_LEVEL_INFO, "[component:info ] CComponentFactories::~CComponentFactories plugins stop");
}

void CComponentFactories::AddFactory( const ComID& component_id, const IComponent::Factory& factory )
{
	XE_LOG_EX(xenon::util::CLog::LOG_LEVEL_INFO, "[component:info ] CComponentRepository::loadPlugin load component(%s)", component_id.c_str());

	FactoryMap::iterator itor = component_factory_list_.find(component_id);
	XE_LOG_EX(xenon::util::CLog::LOG_LEVEL_INFO, "[component:info ] CComponentRepository::loadPlugin load component(%s) 1", component_id.c_str());

	if(itor != component_factory_list_.end())
	{	
		XE_LOG_EX(xenon::util::CLog::LOG_LEVEL_WARN, "[component:warn ] CComponentRepository::~loadPlugin component(%s) existed��replace it", component_id.c_str());
		component_factory_list_.erase(itor);
	}

	XE_LOG_EX(xenon::util::CLog::LOG_LEVEL_INFO, "[component:info ] CComponentRepository::loadPlugin load component(%s) 2", component_id.c_str());

	FactoryInfoPtr temp_factory(new FactoryInfo);
	temp_factory->enable = true;

	XE_LOG_EX(xenon::util::CLog::LOG_LEVEL_INFO, "[component:info ] CComponentRepository::loadPlugin load component(%s) 3", component_id.c_str());

	temp_factory->component_factory	= factory;
	component_factory_list_.insert(make_pair(component_id, temp_factory));

	XE_LOG_EX(xenon::util::CLog::LOG_LEVEL_WARN, "[component:warn ] CComponentRepository::loadPlugin component(%s) finished", component_id.c_str());
}

void CComponentFactories::AddFactory( const ComID& component_id, const IComponent::Factory& factory, CDynamicLib::Ptr& dynamic_lib_ptr )
{
	XE_LOG_EX(xenon::util::CLog::LOG_LEVEL_INFO, "[component:info ] CComponentRepository::loadPlugin load component(%s)", component_id.c_str());

	boost::unique_lock<boost::shared_mutex> lock(mutex_factory_list_);
	FactoryMap::iterator itor = component_factory_list_.find(component_id);
	if(itor != component_factory_list_.end())
	{	
		XE_LOG_EX(xenon::util::CLog::LOG_LEVEL_WARN, "[component:warn ] CComponentRepository::~loadPlugin component(%s) existed��replace it", component_id.c_str());
		component_factory_list_.erase(itor);
	}

	/*
	 *	�Ѳ�������ø�����һ�ݣ�ֻҪ�������ͷŲ���Ͳ����ͷ�
	 */
	FactoryInfoPtr temp_factory(new FactoryInfo);
	temp_factory->enable = true;
	temp_factory->component_factory	= factory;
	temp_factory->denpend_dyn_lib = dynamic_lib_ptr;
	component_factory_list_.insert(make_pair(component_id, temp_factory));
}

void CComponentFactories::RemoveFactory( const ComID& component_id )
{
	XE_LOG_EX(xenon::util::CLog::LOG_LEVEL_INFO, "[component:info ] CComponentRepository::unloadPlugin unload component(%s)", component_id.c_str());

	boost::unique_lock<boost::shared_mutex> lock(mutex_factory_list_);
	FactoryMap::iterator itor = component_factory_list_.find(component_id);
	if(itor == component_factory_list_.end())
	{	
		XE_LOG_EX(xenon::util::CLog::LOG_LEVEL_ERROR, "[component:error] CComponentRepository::unloadPlugin unload component(%s) failed [ component doesnt existed ]", component_id.c_str());
	}
	component_factory_list_.erase(itor);
}


IComponent::Ptr CComponentFactories::Create( const ComID& component_id )
{
	boost::shared_lock<boost::shared_mutex> lock(mutex_factory_list_);
	FactoryMap::iterator itor = component_factory_list_.find(component_id);
	if( itor == component_factory_list_.end() )
	{	
		XE_LOG_EX(xenon::util::CLog::LOG_LEVEL_ERROR, "[component:error] CComponentRepository::createPlugin create component (%s) failed [ component doesnt existed ]", component_id.c_str());
		return IComponent::Ptr();
	}

	if( itor->second->enable == false )
	{
		XE_LOG_EX(xenon::util::CLog::LOG_LEVEL_ERROR, "[component:error] CComponentRepository::createPlugin create component (%s) failed [component is disable]", component_id.c_str());
		return IComponent::Ptr();
	}

	/*
	 *	�˴�������ʱ���ѹ��������ø����һ�ݣ�������ͷŹ����Ͳ����ͷţ������ǲ��ͷŲ���Ͳ����ͷš�
	 */
	return IComponent::Ptr(itor->second->component_factory(), boost::bind(&CComponentFactories::Destroy, this, itor->second, _1));
}

void CComponentFactories::Destroy( FactoryInfoPtr factory_info, IComponent* component )
{
	delete component;
}

void CComponentFactories::Enable( const ComID& component_id )
{
	XE_LOG_EX(xenon::util::CLog::LOG_LEVEL_INFO, "[component:info ] CComponentRepository::enablePlugin enable plugin (%s)", component_id.c_str());

	boost::shared_lock<boost::shared_mutex> lock(mutex_factory_list_);

	FactoryMap::iterator itor = component_factory_list_.find(component_id);

	if( itor == component_factory_list_.end() )
	{
		XE_LOG_EX(xenon::util::CLog::LOG_LEVEL_ERROR, "[component:error] CComponentRepository::enablePlugin  enable component (%s) failed[ component doesnt existed ]", component_id.c_str());
		return;
	}

	itor->second->enable = true;
}

void CComponentFactories::Disable( const ComID& component_id )
{
	XE_LOG_EX(xenon::util::CLog::LOG_LEVEL_INFO, "[component:info ] CComponentRepository::disablePlugin disable component (%s)", component_id.c_str());

	boost::shared_lock<boost::shared_mutex> lock(mutex_factory_list_);

	FactoryMap::iterator itor = component_factory_list_.find(component_id);

	if( itor == component_factory_list_.end() )
	{
		XE_LOG_EX(xenon::util::CLog::LOG_LEVEL_ERROR, "[component:error] CComponentRepository::disablePlugin disable component (%s) failed [ component doesnt existed ]", component_id.c_str());
		return;
	}

	itor->second->enable = false;
}

bool CComponentFactories::IsEnable( const ComID& component_id )
{
	boost::shared_lock<boost::shared_mutex> lock(mutex_factory_list_);
	FactoryMap::iterator itor = component_factory_list_.find(component_id);

	if( itor == component_factory_list_.end() )
		return false;

	return itor->second->enable;
}




#include "precompiled.h"
#include "dxm_factory_manager.h"
#include "dxm_util/util_log.h"

namespace dxm{

	CComFactoryManager::CComFactoryManager()
	{
		sLogInfo("[component:info ] CComponentFactories::CComponentFactories plugins start");

		pthread_mutex_init(&mutex_factory_list_, 0);
	}

	CComFactoryManager::~CComFactoryManager()
	{
		sLogInfo("[component:info ] CComponentFactories::~CComponentFactories plugins stop");

		pthread_mutex_destroy(&mutex_factory_list_);
	}

	void CComFactoryManager::AddFactory(const ComID& component_id, const detail::CComFactory::Ptr& factory)
	{
		sLogInfo("[component:info ] CComponentRepository::loadPlugin load component(%s)", component_id.c_str());  

		pthread_mutex_lock(&mutex_factory_list_);
		FactoryMap::iterator itor = component_factory_list_.find(component_id);	
		if (itor != component_factory_list_.end()){
			sLogWarn("[component:warn ] CComponentRepository::~loadPlugin component(%s) existed，replace it", component_id.c_str());
			component_factory_list_.erase(itor);
		};

		FactoryInfoPtr temp_factory(new FactoryInfo);
		temp_factory->component_factory = factory;
		component_factory_list_.insert(make_pair(component_id, temp_factory));
		pthread_mutex_unlock(&mutex_factory_list_);

		sLogInfo("[component:warn ] CComponentRepository::loadPlugin component(%s) finished", component_id.c_str());
	}

	void CComFactoryManager::AddFactory(const ComID& component_id, const detail::CComFactory::Ptr& factory, CDynamicLib::Ptr& dynamic_lib_ptr)
	{
		sLogInfo("[component:info ] CComponentRepository::loadPlugin load component(%s)", component_id.c_str());

		pthread_mutex_lock(&mutex_factory_list_);
		FactoryMap::iterator itor = component_factory_list_.find(component_id);
		if (itor != component_factory_list_.end()){
			sLogWarn("[component:warn ] CComponentRepository::~loadPlugin component(%s) existed，replace it", component_id.c_str());
			component_factory_list_.erase(itor);
		}

		/*
		 *	把插件的引用给工厂一份，只要工厂不释放插件就不会释放;
		 */
		FactoryInfoPtr temp_factory(new FactoryInfo);
		temp_factory->component_factory = factory;
		temp_factory->denpend_dyn_lib = dynamic_lib_ptr;
		component_factory_list_.insert(make_pair(component_id, temp_factory));
		pthread_mutex_unlock(&mutex_factory_list_);

		sLogInfo("[component:warn ] CComponentRepository::loadPlugin component(%s) finished", component_id.c_str());
	}

	ICom::Ptr CComFactoryManager::CreateCom(const ComID& component_id)
	{
		pthread_mutex_lock(&mutex_factory_list_);
		FactoryMap::iterator itor = component_factory_list_.find(component_id);
		if (itor == component_factory_list_.end())
		{
			sLogError("[component:error] CComponentRepository::createPlugin create component (%s) failed [ component doesnt existed ]", component_id.c_str());
			pthread_mutex_unlock(&mutex_factory_list_);
			return ICom::Ptr();
		}
		/*
		 *	此处创建的时候会把工厂的引用给组件一份，组件不释放工厂就不会释放，工厂是不释放插件就不会释放。
		 */
		ICom::Ptr com_ptr = itor->second->component_factory->CreateCom();
		pthread_mutex_unlock(&mutex_factory_list_);
		return com_ptr;
	} 
}




#ifndef COMPONENT_FACTORIES_H
#define COMPONENT_FACTORIES_H

#include "dxm_com_interface.hpp"
#include "dxm_com_factory.hpp"
#include "dynamic_plugins.h"
#include "pthread/pthread.h"

namespace dxm  {

	class CComFactoryManager
	{
	public:
		struct FactoryInfo
		{
			detail::CComFactory::Ptr component_factory;
			CDynamicLib::Ptr denpend_dyn_lib;
		};

		typedef std::shared_ptr<FactoryInfo> FactoryInfoPtr;
		typedef std::map<ComID, FactoryInfoPtr> FactoryMap;

		CComFactoryManager();
		~CComFactoryManager();

		void AddFactory(const ComID& component_id, const detail::CComFactory::Ptr& factory);  
		void AddFactory(const ComID& component_id, const detail::CComFactory::Ptr& factory, CDynamicLib::Ptr& dynamic_lib_ptr);

		ICom::Ptr CreateCom(const ComID& component_id);

	private:	   
		pthread_mutex_t mutex_factory_list_;
		FactoryMap component_factory_list_;
	};
}
#endif // COMPONENT_FACTORIES_H

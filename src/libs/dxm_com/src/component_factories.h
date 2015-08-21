#ifndef COMPONENT_FACTORIES_H
#define COMPONENT_FACTORIES_H

#include "component_interface.h"
#include "component_plugins.h"

class CComponentFactories
{
public:
	struct FactoryInfo
	{
		bool					enable;
		IComponent::Factory		component_factory;
		CDynamicLib::Ptr		denpend_dyn_lib;
	};

	typedef boost::shared_ptr<FactoryInfo> FactoryInfoPtr;
	typedef map<ComID, FactoryInfoPtr> FactoryMap;

	CComponentFactories();
	
	~CComponentFactories();

	void AddFactory( const ComID& component_id, const IComponent::Factory& factory );

	void AddFactory( const ComID& component_id, const IComponent::Factory& factory, CDynamicLib::Ptr& dynamic_lib_ptr);
	
	void RemoveFactory( const ComID& component_id );
	
	IComponent::Ptr Create( const ComID& component_id );

	void Destroy( FactoryInfoPtr factory_info, IComponent* component );

	void Enable( const ComID& component_id );

	void Disable( const ComID& component_id );

	bool IsEnable( const ComID& component_id );

private:

	boost::shared_mutex mutex_factory_list_;
	FactoryMap component_factory_list_;
};

#endif // COMPONENT_FACTORIES_H

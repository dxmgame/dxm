#ifndef __DXM_COM_CONTEXT_H__
#define __DXM_COM_CONTEXT_H__

#include "dxm_com_lib.h"
#include "dxm_com_interface.hpp"
#include "dxm_com_factory.hpp"

NS_DXM_BEGIN

class CDynamicLib;
class CDynamicPlugins;
class CComFactoryManager;

namespace detail{
	class DXM_COM_DECL CComContext{

	public:
		CComContext();
		~CComContext();

		void SetLog(const std::function<void(int, const std::string&)>& log_callback);

		void LoadPlugin(const std::string& plugin_name);

		void AddFactory(const ComID& component_id, const detail::CComFactory::Ptr& factory);
		void AddFactory(const ComID& component_id, const detail::CComFactory::Ptr& factory, std::shared_ptr<CDynamicLib>& dynamic_lib_ptr);

		ICom::Ptr CreateCom(const ComID& component_id);

		DXM_SINGLETON(CComContext);

	private:
		CComFactoryManager* factories_;
		CDynamicPlugins* plugins_;
	};
}

NS_DXM_END

#endif // __DXM_COM_CONTEXT_H__

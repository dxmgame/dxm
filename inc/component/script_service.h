#ifndef _____SCRIPT_SERVICE_H_________
#define _____SCRIPT_SERVICE_H_________

#include <string>

#include "dxm_util_boost/util_boost_multi_type_container.hpp"

#include "component_service/component_service.h"
#include "log_service.h"

namespace Foundation {

	namespace ScriptService{

		typedef unsigned char* (*LoadFileFunc)(const std::string& file, unsigned long& size);
		typedef int FunctionHandler;

		/*
		 *	自定义类对象;
		 */
		struct ScriptObject
		{
			void * object_ptr_;
			std::string object_typeid_;

			ScriptObject(){

			}
			template<class T>
			ScriptObject(T * object) {
				object_ptr_ = (void*)object;
				object_typeid_ = typeid(T).name();
			}
		};

		/*
		 *	脚本参数容器;
		 */
		typedef xenon::util_boost::CMultiTypeContainer<std::string, int, float, std::string, ScriptObject> ScriptFunctionArgs;

		/*
		 *	脚本执行体;
		 */
		class IScript : public IComponent
		{
		public:
			typedef boost::shared_ptr<IScript> Ptr;

			IScript(IComponent::Ptr component_depend):component_depend_(component_depend) {}
			virtual ~IScript(void) {}

			// 加载C库;
			virtual int LoadCLib(const char* lib) = 0;

			// 加载脚本;
			virtual int ExecuteFile(const char* file) = 0;
			virtual int ExecuteString(const char* str) = 0;
			virtual int ExecuteBuffer(const char* buffer, int size, const char* name) = 0;

			// 执行全局函数;
			virtual int ExecuteFunction(const char* function, ScriptFunctionArgs& args) = 0;
			virtual int ExecuteFunction(const char* function, ScriptFunctionArgs& args, int& result) = 0;
			virtual int ExecuteFunction(const char* function, ScriptFunctionArgs& args, float& result) = 0;
			virtual int ExecuteFunction(const char* function, ScriptFunctionArgs& args, std::string& result) = 0;

			// 执行事件回调;
			virtual int ExecuteFunctionHandler(FunctionHandler handler, ScriptFunctionArgs& args) = 0;
			virtual int ExecuteFunctionHandler(FunctionHandler handler, ScriptFunctionArgs& args, int& result) = 0;
			virtual int ExecuteFunctionHandler(FunctionHandler handler, ScriptFunctionArgs& args, float& result) = 0;
			virtual int ExecuteFunctionHandler(FunctionHandler handler, ScriptFunctionArgs& args, std::string& result) = 0;

		private:
			
			// 潜规则：由组件创建产生给应用层使用的对象，该对象必须引用该组件;
			IComponent::Ptr component_depend_; 
		};

		/*
		 *	脚本服务;
		 */
		class IScriptService : public IComponent
		{
		public:
			typedef boost::shared_ptr<IScriptService> Ptr;

			virtual ~IScriptService(void) {}

			// 重载文件加载函数;
			virtual void SetLoadFileFunc(LoadFileFunc load_file_func) = 0;

			// 创建脚本运行者;
			virtual IScript::Ptr CreateScript(void) = 0;
		};

	};

};

#endif

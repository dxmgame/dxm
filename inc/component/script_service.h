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
		 *	�Զ��������;
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
		 *	�ű���������;
		 */
		typedef xenon::util_boost::CMultiTypeContainer<std::string, int, float, std::string, ScriptObject> ScriptFunctionArgs;

		/*
		 *	�ű�ִ����;
		 */
		class IScript : public IComponent
		{
		public:
			typedef boost::shared_ptr<IScript> Ptr;

			IScript(IComponent::Ptr component_depend):component_depend_(component_depend) {}
			virtual ~IScript(void) {}

			// ����C��;
			virtual int LoadCLib(const char* lib) = 0;

			// ���ؽű�;
			virtual int ExecuteFile(const char* file) = 0;
			virtual int ExecuteString(const char* str) = 0;
			virtual int ExecuteBuffer(const char* buffer, int size, const char* name) = 0;

			// ִ��ȫ�ֺ���;
			virtual int ExecuteFunction(const char* function, ScriptFunctionArgs& args) = 0;
			virtual int ExecuteFunction(const char* function, ScriptFunctionArgs& args, int& result) = 0;
			virtual int ExecuteFunction(const char* function, ScriptFunctionArgs& args, float& result) = 0;
			virtual int ExecuteFunction(const char* function, ScriptFunctionArgs& args, std::string& result) = 0;

			// ִ���¼��ص�;
			virtual int ExecuteFunctionHandler(FunctionHandler handler, ScriptFunctionArgs& args) = 0;
			virtual int ExecuteFunctionHandler(FunctionHandler handler, ScriptFunctionArgs& args, int& result) = 0;
			virtual int ExecuteFunctionHandler(FunctionHandler handler, ScriptFunctionArgs& args, float& result) = 0;
			virtual int ExecuteFunctionHandler(FunctionHandler handler, ScriptFunctionArgs& args, std::string& result) = 0;

		private:
			
			// Ǳ�������������������Ӧ�ò�ʹ�õĶ��󣬸ö���������ø����;
			IComponent::Ptr component_depend_; 
		};

		/*
		 *	�ű�����;
		 */
		class IScriptService : public IComponent
		{
		public:
			typedef boost::shared_ptr<IScriptService> Ptr;

			virtual ~IScriptService(void) {}

			// �����ļ����غ���;
			virtual void SetLoadFileFunc(LoadFileFunc load_file_func) = 0;

			// �����ű�������;
			virtual IScript::Ptr CreateScript(void) = 0;
		};

	};

};

#endif

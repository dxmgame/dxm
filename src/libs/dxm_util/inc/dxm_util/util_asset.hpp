#ifndef __DXM_UTIL_ASSET_HPP__
#define __DXM_UTIL_ASSET_HPP__

#include "dxm_util_lib.h"

#include <map>
#include <list>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_iterators.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "rapidxml/rapidxml_utils.hpp"	

#include "dxm_util/util_log.h"	
#include "dxm/platform.h"

NS_DXM_BEGIN
NS_UTIL_BEGIN

/// \brief ��Դ��ȡ��;
/// 
/// ��ϸ˵�����ض����ļ���ȡ;����cocos2d�ļ���ȡ�ض���;�ļ����ܶ�ȡ�ض���;

class DXM_UTIL_DECL CAssetLoader{
public:
	DXM_SINGLETON(CAssetLoader);
	DXM_REF_PROPERTY(std::function<void*(const char*, dxm_boolean, unsigned long*)>, read_file_function);
	DXM_REF_PROPERTY(std::function<void(void*)>, free_file_data_function);
	DXM_REF_PROPERTY(std::function<std::string(const std::string&)>, full_path_for_file_name_function);

	void* ReadFile(const char* file_name, dxm_boolean is_async, unsigned long* size){
		if(read_file_function_){
			return read_file_function_(file_name, is_async, size);
		}

		unsigned char * pBuffer = NULL;
		*size = 0;
		do {

			// read the file from hardware
			FILE *fp = fopen(file_name, "rb");
			DXM_BREAK_IF(!fp);

			fseek(fp,0,SEEK_END);
			*size = ftell(fp);
			fseek(fp,0,SEEK_SET);
			pBuffer = new unsigned char[*size];
			*size = fread(pBuffer,sizeof(unsigned char), *size,fp);
			fclose(fp);
		} while (0);
		return pBuffer;
	}

	void FreeFileData(void* data){
		if(free_file_data_function_){
			free_file_data_function_(data);
			return;
		}
		DXM_SAFE_DELETE_ARRAY(data);
		return;
	}

	std::string FullPathForFileName(const std::string& file_name){
		if(full_path_for_file_name_function_){
			return full_path_for_file_name_function_(file_name);
		}
		return file_name;
	}
private:
	CAssetLoader() {}
};

/// \brief ��Դ���ػ���ȡ;
/// 
/// ��ϸ˵��;

class DXM_UTIL_DECL CAssetLocale{
public:
	DXM_SINGLETON(CAssetLocale);
	DXM_REF_PROPERTY(std::function<std::string(dxm_int32)>, locale_get_string);
private:
	CAssetLocale() {}
};


/// \brief ��Դ�ڵ�;
/// 
/// ��ϸ˵����rapidxml::xml_node�İ�װ��;

class DXM_UTIL_DECL CAssetNode
{
public:
	CAssetNode(void) {
		node_ = 0;
	}

	CAssetNode(rapidxml::xml_node<>* node) {
		node_ = node;
	}

	CAssetNode(const CAssetNode& node) {
		node_ = node.node_;
	}

	// ��ȡ�ӽڵ���ֵ;
	template<class T>
	T Get(const std::string& key, T default_value) const {

		if(!node_) {
			return default_value;
		}
		
		rapidxml::xml_node<>* temp_node = node_->first_node(key.c_str());
		if(!temp_node || temp_node->value_size() == 0) {
			return default_value;
		}
		return dxm::util::lexical_cast<T>(temp_node->value());
	}

	bool Get(const std::string& key, bool default_value) const {

		if(!node_) {
			return default_value;
		}

		rapidxml::xml_node<>* temp_node = node_->first_node(key.c_str());
		if(!temp_node || temp_node->value_size() == 0) {
			return default_value;
		}
		return (strcmp(temp_node->value(), "true") == 0 ? true : false);
	}

	std::string Get(const std::string& key, std::string default_value) const {

		// û�нڵ��򷵻�Ĭ��ֵ;
		if(!node_) {
			return default_value;
		}

		// ���Ա��ػ���ȡ;
		if(CAssetLocale::GetInstancePtr()->locale_get_string()){
			std::string locale_key = "_"+key;
			rapidxml::xml_node<>* temp_node = node_->first_node(locale_key.c_str());
			if(temp_node && temp_node->value_size() > 0){
				dxm_int32 string_id = dxm::util::lexical_cast<dxm_int32>(temp_node->value());;			
				std::string string_value = CAssetLocale::GetInstancePtr()->locale_get_string()(string_id);
				if(string_value.size()>0){
					return string_value;
				}
			}
		}

		// ��ȡ����������;
		rapidxml::xml_node<>* temp_node = node_->first_node(key.c_str());
		if(!temp_node || temp_node->value_size() == 0) {
			return default_value;
		}
		return dxm::util::lexical_cast<std::string>(temp_node->value());
	}

	// ��ȡ�ӽڵ�����;
	template<class T>
	T GetAttribute(const std::string& key, T default_value) const {

		if(!node_) {
			return default_value;
		}

		rapidxml::xml_attribute<>* temp_node = node_->first_attribute(key.c_str());
		if(!temp_node || temp_node->value_size() == 0) {
			return default_value;
		}
		return dxm::util::lexical_cast<T>(temp_node->value());
	}

	bool GetAttribute(const std::string& key, bool default_value) const {

		if(!node_) {
			return default_value;
		}

		rapidxml::xml_attribute<>* temp_node = node_->first_attribute(key.c_str());
		if(!temp_node || temp_node->value_size() == 0) {
			return default_value;
		}
		return (strcmp(temp_node->value(), "true") == 0 ? true : false);
	}

	// ��ȡ�׸��ӽڵ�;
	CAssetNode GetChild(void) const {

		CAssetNode node;

		if(node_) {

			rapidxml::xml_node<>* temp_node = node_->first_node();
			if(temp_node) {
				node.node_ = temp_node;
			}
		}
		return node;
	}

	// ��ȡ�ӽڵ�;
	CAssetNode GetChild(const std::string& key) const {
		
		CAssetNode node;

		if(node_) {

			rapidxml::xml_node<>* temp_node = node_->first_node(key.c_str());
			if(temp_node) {
				node.node_ = temp_node;
			}
		}
		return node;
	}

	// ��ȡ�ھӽڵ�;
	CAssetNode GetSibling(void) const {

		CAssetNode node;

		if(node_) {

			rapidxml::xml_node<>* temp_node = node_->next_sibling();
			if(temp_node) {
				node.node_ = temp_node;
			}
		}
		return node;
	}

	// ��ȡ�ھӽڵ�;
	CAssetNode GetSibling(const std::string& key) const {

		CAssetNode node;

		if(node_) {

			rapidxml::xml_node<>* temp_node = node_->next_sibling(key.c_str());
			if(temp_node) {
				node.node_ = temp_node;
			}
		}
		return node;
	}

	// ��ȡ����;
	char* GetName(void) const {

		if(node_) {
			return node_->name();
		}
		return "";
	}

	// ��ȡ��ֵ;
	template<class T>
	T GetValue(T default_value) const {

		if(!node_ || node_->value_size() == 0) {
			return default_value;
		}
		return dxm::util::lexical_cast<T>(node_->value());
	}

	bool GetValue(bool default_value) const {

		if(!node_ || node_->value_size() == 0) {
			return default_value;
		}
		return (strcmp(node_->value(), "true") == 0 ? true : false);
	}

	// ��ȡXML�ڵ�;
	inline rapidxml::xml_node<>* node(void) const { return node_; }

	// �ж��Ƿ�Ϊ��;
	inline bool operator!(void) const { return !node_; }

	// ����ָ��boolֵ������;
	typedef rapidxml::xml_node<>* CAssetNode::*unspecified_bool;
	operator unspecified_bool() const { return !node_ ? 0 : &CAssetNode::node_; }

private:
	rapidxml::xml_node<>* node_; // XML�ڵ�;
};

/// \brief ���ݼ�����;
/// 
/// ��ϸ˵����������Ҫʵ��LoadPtree��Link; ���๦��Ϊ����Data����LoadPtreeʵ��Data�ڹ���; 
///
/// ��������ģ�壺���ζ�ȡP1,P2��;
/// 
/// <Data>
/// 	<P1/>
/// 	<P2/>
/// 	...
/// </Data>
class DXM_UTIL_DECL CAssetData
{
public:
	typedef std::shared_ptr<CAssetData> Ptr;

	// ��������;
	virtual ~CAssetData(void) {}

	// ���ļ��ж�ȡ;
	bool LoadFile(const std::string& file_name, dxm_boolean is_async = false) {

		bool is_successful = false;
		try {

			// ��ȡ�ļ�;
			unsigned long buffer_size;
			void* buffer = CAssetLoader::GetInstancePtr()->ReadFile(file_name.c_str(), is_async, &buffer_size);
			if(buffer){
				is_successful = LoadData(buffer, buffer_size);
				CAssetLoader::GetInstancePtr()->FreeFileData((void*)buffer);
			}
		}
		catch (std::exception& e) {

			// Assets Error
			sLogError("<Assets Error> read error!(%s)", e.what());
			is_successful = false;
		}

		sLogInfo("%s load %s!", file_name.c_str(), is_successful ? "successful" : "failed");
		return is_successful;
	}

	// ���ڴ��ж�ȡ;
	bool LoadData(const void* data, dxm_uint32 size) {

		bool is_successful = false;
		try {

			// �����ļ�;
			rapidxml::xml_document<> pt;
			pt.parse<0>((char*)data, size);

			rapidxml::xml_node<>* child = pt.first_node("Data");
			if(child){
				is_successful = this->LoadPtree(CAssetNode(child));
			}
			else {

				// Assets Error
				sLogError("<Assets Error> read error(file format is invalidate)");
				is_successful = false;
			}
		}
		catch( std::runtime_error e) {

			// Assets Error
			sLogError("<Assets Error> read error!(%s)", e.what());
			is_successful = false;
		}
		return is_successful;
	}

	// ��ptree�ж�ȡ;
	virtual bool LoadPtree(const CAssetNode& pt) {
		return true;
	}

	// ���Ӻ���;
	virtual bool Link(void) {
		return true;
	}
};


/// \brief ��Ŀ������;
/// 
/// ��ϸ˵����Data/Object�����ϣ�����ͨ��excel���뵼��������߻�����;
/*
 *	���ʽ��������ģ��;
	<Data>
		<Object>
			<ID>1</ID>
		</Object>
		<Object>
			<ID>2</ID>
		</Object>
	</Data>
 */
template<class IDType, class EntryType>
class CAssetMap : public std::map< IDType, std::shared_ptr<EntryType> >, public CAssetData
{
public:

	// ��������;
	virtual ~CAssetMap(void) {}

	// ��ptree�ж�ȡ;
	virtual bool LoadPtree(const CAssetNode& node) {

		bool is_successful = true;

		try {

			rapidxml::xml_node<>* temp_node = node.node();
			if(!temp_node) {
				return false;
			}

			for(rapidxml::xml_node<>* pos = temp_node->first_node("Object"); pos; pos = pos->next_sibling("Object")) {

				// ������Ŀ��Ʒ�����м���;
				std::shared_ptr<EntryType> asset_entry(new EntryType);
				if(!asset_entry || !asset_entry->Load(CAssetNode(pos))) {
					is_successful = false;
					continue;
				}

				// �����Ŀ;
				AddEntry(asset_entry);
			}
		}
		catch(std::runtime_error e) {

			// Assets Error
			sLogError("<Assets Error> read error!(%s)", e.what());
			is_successful = false;
		}
		return is_successful;
	}

	// ��ȡ��Ŀ;
	std::shared_ptr<EntryType> Get(IDType id) {

		typename std::map<IDType, std::shared_ptr<EntryType> >::iterator itor = this->find(id);
		if(itor != this->end()) {
			return itor->second;
		}
		return std::shared_ptr<EntryType>();
	}

	// ������Ŀ;
	bool Link(void) {

		bool is_successful = true;

		typename std::map<IDType, std::shared_ptr<EntryType> >::iterator itor = this->begin();
		while(itor != this->end()) {

			if(!itor->second->Link()) {
				is_successful = false;
			}
			++itor;
		}
		return is_successful;
	}

	// �����Ŀ;
	virtual void AddEntry(std::shared_ptr<EntryType> asset_entry) {
		auto itor = this->find(asset_entry->id());
		if(itor != this->end()){
			this->erase(itor);
		}
		this->insert(std::make_pair(asset_entry->id(), asset_entry));
	}
};

NS_UTIL_END
NS_DXM_END

#endif
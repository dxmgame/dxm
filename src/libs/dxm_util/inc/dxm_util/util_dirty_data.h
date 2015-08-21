/*
 *	����������;
		������������ʽ������Դ�����������ݺ󣬵�һ�α�����Ϊ������ݽ������浽�������б��С�
	�ⲿֻ��Ҫ�Ը��б���б��������Ϳ��ԣ������ñ�������������;
 *
 */
#ifndef __DXM_UTIL_DIRTY_DATA_H__
#define __DXM_UTIL_DIRTY_DATA_H__

#include "dxm_util_lib.h"
#include <list>
#include "util_bit_array.h"

NS_DXM_BEGIN
NS_UTIL_BEGIN


/// \brief �������б�;
/// 
/// ��ϸ˵��: ʵ�������ݵ����ü��������б��ȡ;

class DXM_UTIL_DECL CDirtyData
{
public:
	CDirtyData(void);
	~CDirtyData(void);

	/// \brief ��ʼ��;
	void Resize(dxm_uint32 size);
	/// \brief ����������;
	void SetDirty(dxm_uint32 index);
	/// \brief ����������;
	bool GetDirty(dxm_uint32 index);
	/// \brief �Ƿ����������;
	bool HasDirty(void);	
	/// \brief ����������;
	void ClearDirty(void);
	/// \brief ���������������б�;
	inline const std::list<dxm_uint32>& dirty_index_list(void) { return dirty_index_list_; }
	/// \brief ��������;string format��size+data;
	void ParseFromString(const std::string& input);
	/// \brief �������ݵ��ַ���;
	bool SerializeToString(std::string* output);

	dxm_uint32 size();
private:
	
	CBitArray dirty_;
	std::list<dxm_uint32> dirty_index_list_;
};

NS_UTIL_END
NS_DXM_END

#endif // __DXM_UTIL_DIRTY_LIST_H__

/*
 *	脏数据助手;
		适用于数组形式的数据源，设置脏数据后，第一次被设置为脏的数据将被保存到脏数据列表中。
	外部只需要对该列表进行遍历操作就可以，而不用遍历脏数据数组;
 *
 */
#ifndef __DXM_UTIL_DIRTY_DATA_H__
#define __DXM_UTIL_DIRTY_DATA_H__

#include "dxm_util_lib.h"
#include <list>
#include "util_bit_array.h"

NS_DXM_BEGIN
NS_UTIL_BEGIN


/// \brief 脏数据列表;
/// 
/// 详细说明: 实现脏数据的设置及脏数据列表获取;

class DXM_UTIL_DECL CDirtyData
{
public:
	CDirtyData(void);
	~CDirtyData(void);

	/// \brief 初始化;
	void Resize(dxm_uint32 size);
	/// \brief 设置脏数据;
	void SetDirty(dxm_uint32 index);
	/// \brief 返回脏数据;
	bool GetDirty(dxm_uint32 index);
	/// \brief 是否存在脏数据;
	bool HasDirty(void);	
	/// \brief 清理脏数据;
	void ClearDirty(void);
	/// \brief 返回脏数据索引列表;
	inline const std::list<dxm_uint32>& dirty_index_list(void) { return dirty_index_list_; }
	/// \brief 加载数据;string format：size+data;
	void ParseFromString(const std::string& input);
	/// \brief 保存数据到字符串;
	bool SerializeToString(std::string* output);

	dxm_uint32 size();
private:
	
	CBitArray dirty_;
	std::list<dxm_uint32> dirty_index_list_;
};

NS_UTIL_END
NS_DXM_END

#endif // __DXM_UTIL_DIRTY_LIST_H__

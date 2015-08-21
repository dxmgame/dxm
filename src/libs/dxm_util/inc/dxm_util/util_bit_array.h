#ifndef __DXM_UTIL_BIT_ARRAY_H__ 
#define __DXM_UTIL_BIT_ARRAY_H__

#include "dxm_util_lib.h"

NS_DXM_BEGIN
NS_UTIL_BEGIN

/// \brief 位布尔数组容器; 
/// 
/// 详细说明：为节省内存的而引出的类;
class DXM_UTIL_DECL CBitArray{
public:
	CBitArray( );
	CBitArray( dxm_uint32 size );
	~CBitArray();
	/// \brief 重置位容器大小;
	void Resize(dxm_uint32 size);
	/// \brief 设置对应数据位内容;
	void Set( dxm_uint32 index, dxm_boolean val );
	/// \brief 获取对应数据位内容;
	dxm_boolean Get( dxm_uint32 index );
	/// \brief 清理所有数据位内容;
	void Clear();
	/// \brief 加载数据;string format：size+data;
	void ParseFromString(const std::string& input);
	/// \brief 保存数据到字符串;
	bool SerializeToString(std::string* output);

	DXM_PROPERTY(dxm_uint32, size);
	DXM_PROPERTY(dxm_uint32, byte_size);
private:
	dxm_uint8* data_; ///< 内存地址;
};

NS_UTIL_END
NS_DXM_END

#endif // __DXM_UTIL_BIT_ARRAY_H__
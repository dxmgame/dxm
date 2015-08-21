#ifndef __DXM_UTIL_BIT_TABLE_H__ 
#define __DXM_UTIL_BIT_TABLE_H__

#include "dxm_util_lib.h"

NS_DXM_BEGIN
NS_UTIL_BEGIN

/// \brief 位布尔二维数组容器; 
/// 
/// 详细说明：为节省内存的而引出的类;
class DXM_UTIL_DECL CBitTable{
public:
	CBitTable();
	CBitTable( dxm_uint32 row_size, dxm_uint32 column_size  );
	~CBitTable();
	/// \brief 重置位容器大小;
	void Resize( dxm_uint32 row_size, dxm_uint32 column_size );
	/// \brief 设置对应数据位内容;
	void Set( dxm_uint32 row, dxm_uint32 column, dxm_boolean val );
	/// \brief 获取对应数据位内容;
	dxm_boolean Get( dxm_uint32 row, dxm_uint32 column );
	/// \brief 清理所有数据位内容;
	void Clear();
	/// \brief 加载数据;string format：bit_size+data;
	void ParseFromString(const std::string& input);
	/// \brief 保存数据到字符串;
	bool SerializeToString(std::string* output);

	DXM_PROPERTY(dxm_uint32, row_size);
	DXM_PROPERTY(dxm_uint32, column_size);
	DXM_PROPERTY(dxm_uint32, byte_size);
private:
	dxm_uint32 size_; ///< 字节数量;
	dxm_uint8* data_; ///< 内存地址;
};

NS_UTIL_END
NS_DXM_END

#endif // __DXM_UTIL_BIT_TABLE_H__
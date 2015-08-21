#ifndef __DXM_UTIL_BIT_TABLE_H__ 
#define __DXM_UTIL_BIT_TABLE_H__

#include "dxm_util_lib.h"

NS_DXM_BEGIN
NS_UTIL_BEGIN

/// \brief λ������ά��������; 
/// 
/// ��ϸ˵����Ϊ��ʡ�ڴ�Ķ���������;
class DXM_UTIL_DECL CBitTable{
public:
	CBitTable();
	CBitTable( dxm_uint32 row_size, dxm_uint32 column_size  );
	~CBitTable();
	/// \brief ����λ������С;
	void Resize( dxm_uint32 row_size, dxm_uint32 column_size );
	/// \brief ���ö�Ӧ����λ����;
	void Set( dxm_uint32 row, dxm_uint32 column, dxm_boolean val );
	/// \brief ��ȡ��Ӧ����λ����;
	dxm_boolean Get( dxm_uint32 row, dxm_uint32 column );
	/// \brief ������������λ����;
	void Clear();
	/// \brief ��������;string format��bit_size+data;
	void ParseFromString(const std::string& input);
	/// \brief �������ݵ��ַ���;
	bool SerializeToString(std::string* output);

	DXM_PROPERTY(dxm_uint32, row_size);
	DXM_PROPERTY(dxm_uint32, column_size);
	DXM_PROPERTY(dxm_uint32, byte_size);
private:
	dxm_uint32 size_; ///< �ֽ�����;
	dxm_uint8* data_; ///< �ڴ��ַ;
};

NS_UTIL_END
NS_DXM_END

#endif // __DXM_UTIL_BIT_TABLE_H__
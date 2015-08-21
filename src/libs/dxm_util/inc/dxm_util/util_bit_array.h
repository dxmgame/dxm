#ifndef __DXM_UTIL_BIT_ARRAY_H__ 
#define __DXM_UTIL_BIT_ARRAY_H__

#include "dxm_util_lib.h"

NS_DXM_BEGIN
NS_UTIL_BEGIN

/// \brief λ������������; 
/// 
/// ��ϸ˵����Ϊ��ʡ�ڴ�Ķ���������;
class DXM_UTIL_DECL CBitArray{
public:
	CBitArray( );
	CBitArray( dxm_uint32 size );
	~CBitArray();
	/// \brief ����λ������С;
	void Resize(dxm_uint32 size);
	/// \brief ���ö�Ӧ����λ����;
	void Set( dxm_uint32 index, dxm_boolean val );
	/// \brief ��ȡ��Ӧ����λ����;
	dxm_boolean Get( dxm_uint32 index );
	/// \brief ������������λ����;
	void Clear();
	/// \brief ��������;string format��size+data;
	void ParseFromString(const std::string& input);
	/// \brief �������ݵ��ַ���;
	bool SerializeToString(std::string* output);

	DXM_PROPERTY(dxm_uint32, size);
	DXM_PROPERTY(dxm_uint32, byte_size);
private:
	dxm_uint8* data_; ///< �ڴ��ַ;
};

NS_UTIL_END
NS_DXM_END

#endif // __DXM_UTIL_BIT_ARRAY_H__
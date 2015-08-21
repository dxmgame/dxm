#ifndef __DXM_UTIL_BUFFER_H__
#define __DXM_UTIL_BUFFER_H__

#include "dxm_util_lib.h"
#include <memory>

#define INTERNAL_BUFFER_MALLOC malloc
#define INTERNAL_BUFFER_FREE free

NS_DXM_BEGIN
NS_UTIL_BEGIN

namespace detail{
	/// \brief ����ָ������������;
	/// 
	/// ��ϸ˵��: ʹ�ù�����ʱ;
	class CByteArrayMemoryRef{
	public:
		CByteArrayMemoryRef(void* ptr);
		~CByteArrayMemoryRef();
	private:
		void* ptr;
	};
}

/// \brief Buffer���ͣ����������ֶ��ͷ�(external)�������Զ��ͷ�(internal);
/// 
/// ��ϸ˵��;
enum ByteArrayType
{
	BUFF_TYPE_EXTERNAL = 0, // �����ֶ��ͷ�;�ڲ�������ָ����ͷ�;
	BUFF_TYPE_INTERNAL, // �����Զ��ͷ�;
	BUFF_TYPE_MAX
};


/// \brief �ڴ滺����;
/// 
/// ��ϸ˵�������ڱ������ݵ���ʱλ�ã���CSerializer����ʹ��;
class DXM_UTIL_DECL CByteArray
{
public:
	CByteArray();
	explicit CByteArray(size_t elemSize);
	CByteArray(const CByteArray& other);
	CByteArray(const CByteArray& other, size_t offset, size_t len);
	CByteArray(const void* ptr, size_t len, size_t elemSize = sizeof(char));
	CByteArray(size_t len, size_t elemSize = sizeof(char));
	~CByteArray();

	CByteArray& operator=(const CByteArray& other);

public:
	void* ptr();
	const void* ptr() const;
	void* ptr(size_t offset);
	const void* ptr(size_t offset) const;
	void* raw();
	const void* raw() const;
	ByteArrayType type() const;
	size_t size() const;
	size_t elem_size() const;
	bool empty() const;
	operator bool() const;
	bool operator!() const;

private:
	void* raw_;
	void* ptr_;
	size_t size_;
	size_t elem_size_;
	ByteArrayType type_;
	std::shared_ptr<detail::CByteArrayMemoryRef> buffer_ptr_ref_;
};

NS_UTIL_END
NS_DXM_END

#endif // __DXM_UTIL_BUFFER_H__

#ifndef __DXM_UTIL_BUFFER_H__
#define __DXM_UTIL_BUFFER_H__

#include "dxm_util_lib.h"
#include <memory>

#define INTERNAL_BUFFER_MALLOC malloc
#define INTERNAL_BUFFER_FREE free

NS_DXM_BEGIN
NS_UTIL_BEGIN

namespace detail{
	/// \brief 缓冲指针引用清理器;
	/// 
	/// 详细说明: 使用共享缓冲时;
	class CByteArrayMemoryRef{
	public:
		CByteArrayMemoryRef(void* ptr);
		~CByteArrayMemoryRef();
	private:
		void* ptr;
	};
}

/// \brief Buffer类型，包括共享手动释放(external)，共享自动释放(internal);
/// 
/// 详细说明;
enum ByteArrayType
{
	BUFF_TYPE_EXTERNAL = 0, // 共享手动释放;内部不进行指针的释放;
	BUFF_TYPE_INTERNAL, // 共享自动释放;
	BUFF_TYPE_MAX
};


/// \brief 内存缓冲区;
/// 
/// 详细说明：用于保存数据的临时位置，与CSerializer配套使用;
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

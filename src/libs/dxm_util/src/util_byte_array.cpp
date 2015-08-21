#include "precompiled.h"
#include "util_byte_array.h"

NS_DXM_BEGIN
NS_UTIL_BEGIN

detail::CByteArrayMemoryRef::~CByteArrayMemoryRef()
{
	free(ptr);
}

detail::CByteArrayMemoryRef::CByteArrayMemoryRef(void* ptr) : ptr(ptr)
{

}

bool CByteArray::operator!() const
{
	return empty();
}

CByteArray::operator bool() const
{
	return !empty();
}

bool CByteArray::empty() const
{
	return ptr_ == 0 || size_ == 0 ? true : false;
}

size_t CByteArray::elem_size() const
{
	return elem_size_;
}

size_t CByteArray::size() const
{
	return size_;
}

ByteArrayType CByteArray::type() const
{
	return type_;
}

const void* CByteArray::raw() const
{
	return raw_;
}

void* CByteArray::raw()
{
	return raw_;
}

const void* CByteArray::ptr(size_t offset) const
{
	sLogAssert(offset < size_, "ptr:OutOfArrayRange"); 
	return (char*)ptr_ + offset*elem_size_;
}

void* CByteArray::ptr(size_t offset)
{
	sLogAssert(offset < size_, "ptr:OutOfArrayRange"); 
	return (char*)ptr_ + offset*elem_size_;
}

const void* CByteArray::ptr() const
{
	return ptr_;
}

void* CByteArray::ptr()
{
	return ptr_;
}

CByteArray::~CByteArray()
{

}

CByteArray::CByteArray(size_t len, size_t elemSize /*= sizeof(char)*/) :	
	raw_(INTERNAL_BUFFER_MALLOC(len*elemSize)),	ptr_(raw_),	size_(len),	elem_size_(elemSize),
	buffer_ptr_ref_( std::shared_ptr<detail::CByteArrayMemoryRef>(new detail::CByteArrayMemoryRef(const_cast<void*>(ptr_)))), 
	type_(BUFF_TYPE_INTERNAL)
{

}

CByteArray::CByteArray(const void* ptr, size_t len, size_t elemSize /*= sizeof(char)*/) :
	raw_(const_cast<void*>(ptr)),ptr_(const_cast<void*>(ptr)),size_(len),elem_size_(elemSize),type_(BUFF_TYPE_EXTERNAL)
{

}

CByteArray::CByteArray(const CByteArray& other, size_t offset, size_t len) :
	raw_(0),ptr_(0),size_(0),elem_size_(sizeof(char*)),type_(BUFF_TYPE_EXTERNAL)
{
	sLogAssert(offset <= other.size_, "Buffer(const Buffer& other, size_t offset, size_t len):OutOfArray");
	size_t temp_len = other.size_ - offset;
	size_t copyLen = temp_len>len?len:temp_len;
	buffer_ptr_ref_ = other.buffer_ptr_ref_;
	raw_ = other.raw_;
	ptr_ = (char*)other.ptr_ + offset*other.elem_size();
	size_ = copyLen;
	elem_size_ = other.elem_size();
	type_ = other.type_;
}

CByteArray::CByteArray(const CByteArray& other) :
	raw_(0),ptr_(0),size_(0),elem_size_(sizeof(char)),type_(BUFF_TYPE_EXTERNAL)
{
	size_t offset = 0;
	size_t len = other.size_;
	sLogAssert(offset <= other.size_, "Buffer(const Buffer& other):OutOfArray");
	size_t temp_len = other.size_ - offset;
	size_t copyLen = temp_len>len?len:temp_len;
	buffer_ptr_ref_ = other.buffer_ptr_ref_;
	raw_ = other.raw_;
	ptr_ = (char*)other.ptr_ + offset*other.elem_size();
	size_ = copyLen == 0 ? other.size_ - offset : copyLen;
	elem_size_ = other.elem_size();
	type_ = other.type_;
}

CByteArray::CByteArray(size_t elemSize) :
	raw_(0),ptr_(0),size_(0),elem_size_(elemSize),type_(BUFF_TYPE_EXTERNAL)
{

}

CByteArray::CByteArray() : 
	raw_(0),ptr_(0),size_(0),elem_size_(sizeof(char)),type_(BUFF_TYPE_EXTERNAL)
{

}

CByteArray& CByteArray::operator=(const CByteArray& other)
{
	if( this != &other && this->raw() != other.raw()){
		size_t offset = 0;
		size_t len = other.size_;

		sLogAssert(offset <= other.size_, "operator=:OutOfArray");

		size_t temp_len = other.size_ - offset;
		size_t copyLen = temp_len>len?len:temp_len;
		buffer_ptr_ref_ = other.buffer_ptr_ref_;
		raw_ = other.raw_;
		ptr_ = (char*)other.ptr_ + offset*other.elem_size();
		size_ = copyLen == 0 ? other.size_ - offset : copyLen;
		elem_size_ = other.elem_size();
		type_ = other.type_;
	}
	return *this;
}
NS_UTIL_END
NS_DXM_END
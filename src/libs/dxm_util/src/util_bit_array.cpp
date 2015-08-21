
#include "precompiled.h"
#include "util_bit_array.h"

NS_DXM_BEGIN
NS_UTIL_BEGIN

CBitArray::CBitArray( dxm_uint32 size )
{
	if(size==0){
		size_ = 0;
		byte_size_ = 0;
		data_ = 0;
	}
	else{
		size_ = size;
		byte_size_ = (size_+7)>>3;
		data_ = (dxm_uint8*)malloc(byte_size_);
		memset(data_, 0, byte_size_);
	}
}

CBitArray::CBitArray()
{
	size_ = 0;
	byte_size_ = 0;
	data_ = 0;
}

CBitArray::~CBitArray()
{
	if(data_){
		free(data_);
	}
}

void CBitArray::Set( dxm_uint32 index, dxm_boolean val )
{
	if ((index<0) || (index>size_))
		return;

	unsigned char u = (1 << 7) >> (index&7);
	if (val){
		data_[(index>>3)] |= u;
	}
	else{
		data_[(index>>3)] &= ~u;
	}
}

dxm_boolean CBitArray::Get( dxm_uint32 index )
{
	if ((index < 0) || (index >= size_))
		return false;
	dxm_uint8 u = (1 << 7) >> (index&7);
	dxm_uint8 k = this->data_[(index>>3)];
	return ((k & u) != 0);
}

void CBitArray::Clear()
{
	memset( data_, 0, byte_size_ );
}

void CBitArray::ParseFromString( const std::string& input )
{
	const char* input_data = input.data();
	dxm_uint32 bis_size = *((dxm_uint32*)input_data);
	Resize( bis_size );
	input_data += sizeof(dxm_uint32);

	dxm_uint32 input_size = input.size()-sizeof(dxm_uint32);
	memcpy( data_, input_data, byte_size_>input_size?input_size:byte_size_ );
}

bool CBitArray::SerializeToString( std::string* output )
{
	if(!output){
		return false;
	}
	output->resize(byte_size_ + sizeof(dxm_uint32));
	char* output_data = DXM_STRING_PTR(output);
	*((dxm_uint32*)output_data) = size_;
	output_data += sizeof(dxm_uint32);
	memcpy( output_data, data_, byte_size_ );
	return true;
}

void CBitArray::Resize( dxm_uint32 size )
{
	if(size==0){
		if(data_){
			free(data_);
		}
		size_ = 0;
		byte_size_ = 0;
		data_ = 0;
		return;
	}

	dxm_uint8* old_data = data_;
	int old_size = byte_size_;
	if(old_data){
		size_ = size;
		byte_size_ = (size_+7)>>3;
		data_ = (dxm_uint8*)malloc(byte_size_);
		memcpy(data_, old_data, byte_size_>old_size?old_size:byte_size_);
		free(old_data);
	}
	else{
		size_ = size;
		byte_size_ = (size_+7)>>3;
		data_ = (dxm_uint8*)malloc(byte_size_);
		memset(data_, 0, byte_size_);
	}
}

NS_UTIL_END
NS_DXM_END

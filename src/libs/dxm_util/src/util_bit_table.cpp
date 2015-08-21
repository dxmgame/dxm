
#include "precompiled.h"
#include "util_bit_table.h"

NS_DXM_BEGIN
NS_UTIL_BEGIN

// ------------
// CBitTable
CBitTable::CBitTable()
{
	row_size_ = 0;
	column_size_ = 0;
	byte_size_ = 0;
	data_ = 0;
}

CBitTable::CBitTable(dxm_uint32 row_size, dxm_uint32 column_size)
{
	dxm_uint32 bit_size = row_size*column_size;
	if(bit_size==0){
		row_size_ = 0;
		column_size_ = 0;
		byte_size_ = 0;
		data_ = 0;
	}
	else{
		row_size_ = row_size;
		column_size_ = column_size;
		byte_size_ = (bit_size+7)>>3;
		data_ = (dxm_uint8*)malloc(byte_size_);
		memset(data_, 0, byte_size_);
	}
}

CBitTable::~CBitTable()
{
	if(data_){
		free(data_);
	}
}

void CBitTable::Resize(dxm_uint32 row_size, dxm_uint32 column_size)
{
	dxm_uint32 bit_size = row_size*column_size;
	if(bit_size==0){
		if(data_){
			free(data_);
		}
		row_size_ = 0;
		column_size_ = 0;
		byte_size_ = 0;
		data_ = 0;
		return;
	}

	dxm_uint8* old_data = data_;
	int old_size = byte_size_;
	if(old_data){
		row_size_ = row_size;
		column_size_ = column_size;
		byte_size_ = (bit_size+7)>>3;
		data_ = (dxm_uint8*)malloc(byte_size_);
		memcpy(data_, old_data, byte_size_>old_size?old_size:byte_size_);
		free(old_data);
	}
	else{
		row_size_ = row_size;
		column_size_ = column_size;
		byte_size_ = (bit_size+7)>>3;
		data_ = (dxm_uint8*)malloc(byte_size_);
		memset(data_, 0, byte_size_);
	}
}

void CBitTable::Set(dxm_uint32 row, dxm_uint32 column, dxm_boolean val)
{
	if(row>=row_size_){
		return;
	}
	if(column>=column_size_){
		return;
	}

	dxm_uint32 index = row*column_size_+column;

	unsigned char u = (1 << 7) >> (index&7);
	if (val){
		data_[(index>>3)] |= u;
	}
	else{
		data_[(index>>3)] &= ~u;
	}
}

dxm_boolean CBitTable::Get(dxm_uint32 row, dxm_uint32 column)
{
	if(row>row_size_){
		return false;
	}
	if(column>=column_size_){
		return false;
	}

	dxm_uint32 index = row*column_size_+column;
	dxm_uint8 u = (1 << 7) >> (index&7);
	dxm_uint8 k = this->data_[(index>>3)];
	return ((k & u) != 0);
}

void CBitTable::Clear()
{
	memset( data_, 0, byte_size_ );
}

void CBitTable::ParseFromString(const std::string& input)
{
	const char* input_data = input.data();
	dxm_uint32 row_size = *((dxm_uint32*)input_data);
	input_data += sizeof(dxm_uint32);
	dxm_uint32 column_size = *((dxm_uint32*)input_data);
	input_data += sizeof(dxm_uint32);
	Resize( row_size, column_size );

	dxm_uint32 input_size = input.size()-sizeof(dxm_uint32)-sizeof(dxm_uint32);
	memcpy( data_, input_data, byte_size_>input_size?input_size:byte_size_ );
}

bool CBitTable::SerializeToString(std::string* output)
{
	if(!output){
		return false;
	}
	output->resize(byte_size_ + sizeof(dxm_uint32) + sizeof(dxm_uint32));
	char* output_data = DXM_STRING_PTR(output);
	*((dxm_uint32*)output_data) = row_size_;
	output_data += sizeof(dxm_uint32);
	*((dxm_uint32*)output_data) = column_size_;
	output_data += sizeof(dxm_uint32);
	memcpy( output_data, data_, byte_size_ );
	return true;
}

NS_UTIL_END
NS_DXM_END

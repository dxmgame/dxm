#include "precompiled.h"
#include "util_dirty_data.h"

NS_DXM_BEGIN
NS_UTIL_BEGIN

CDirtyData::CDirtyData( void )
{
	dirty_ = 0;
}

CDirtyData::~CDirtyData( void )
{

}

void CDirtyData::Resize( dxm_uint32 size )
{
	dirty_.Resize(size);
}

void CDirtyData::SetDirty( dxm_uint32 index )
{
	if( index < dirty_.size() ) {
		if(!dirty_.Get(index)) {
			dirty_.Set(index, true);
			dirty_index_list_.push_back(index);
		}
	}
}

bool CDirtyData::GetDirty( dxm_uint32 index )
{
	return dirty_.Get(index);
}

bool CDirtyData::HasDirty( void )
{
	return (dirty_index_list_.size() > 0);
}

void CDirtyData::ClearDirty( void )
{
	dirty_.Clear();
	dirty_index_list_.clear();
}

void CDirtyData::ParseFromString(const std::string& input)
{
	dirty_.ParseFromString(input);
	for(int i=0; i<dirty_.size(); ++i){
		if(dirty_.Get(i)){
			dirty_index_list_.push_back(i);
		}
	}
}

bool CDirtyData::SerializeToString(std::string* output)
{
	return dirty_.SerializeToString(output);
}

dxm_uint32 CDirtyData::size()
{
	return dirty_.size();
}

NS_UTIL_END
NS_DXM_END

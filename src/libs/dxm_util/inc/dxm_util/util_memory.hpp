#ifndef __UTIL_MEMORY_H___
#define __UTIL_MEMORY_H___

#include "dxm_util_lib.h"
/*
 *  描述：智能指针强转;
 *	用法;
 *		std::shared_ptr<Base> base_ptr(new Derived);
 *		std::shared_ptr<Derived> derived_ptr = dxm::CMemory::DynamicCast(base_ptr);
 */

NS_DXM_BEGIN
NS_UTIL_BEGIN
	namespace detail{
		template <typename Derived, typename Base>
		class CDynamicCastDeleter
		{
		public:
			CDynamicCastDeleter(const std::shared_ptr<Base>& base_ptr) { base_ptr_ = base_ptr; }
			~CDynamicCastDeleter() {}

			/// operator (), 清理器
			void operator()(Derived* ptr){
				base_ptr_.reset();
			}

		private:
			std::shared_ptr<Base> base_ptr_;
		};
}

class CMemory{

	template <typename Derived, typename Base>
	static std::shared_ptr<Derived> DynamicCast(const std::shared_ptr<Base>& base_ptr){
		Derived* derived_pt = dynamic_cast<Derived*>(base_ptr.get());
		if(derived_pt){
			return std::shared_ptr<Derived>( derived_pt, detail::CDynamicCastDeleter<Derived, Base>(base_ptr) );
		}
		return std::shared_ptr<Derived>();
	}
};
NS_UTIL_END
NS_DXM_END

#endif // __UTIL_MEMORY_H___

#pragma once

#ifndef __DXM_COM_FACTORY_H___
#define __DXM_COM_FACTORY_H___

#include "dxm_com_interface.hpp"

namespace dxm{
	namespace detail{
		class CComFactory{
		public:
			typedef std::shared_ptr<CComFactory> Ptr;

			virtual ~CComFactory() {};
			virtual ICom::Ptr CreateCom() = 0;

			template <class T>  static CComFactory::Ptr CreateFactory();
		};
		// 
		template <class T>
		class TComFactory : public CComFactory{
		public:
			virtual ~TComFactory() {};	 
			virtual ICom::Ptr CreateCom() { return std::shared_ptr<T>(_Malloc(), _Free); }
			// 
		private:
			static T* _Malloc(){ return new T; }
			static void _Free(T* object){ delete object; }
		};

		template <class T> static CComFactory::Ptr CComFactory::CreateFactory()
		{
			return std::shared_ptr< TComFactory<T> >(new TComFactory<T>, [](TComFactory<T>* object){ delete object; });
		}
	}
}

#endif
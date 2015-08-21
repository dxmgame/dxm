#pragma once
#ifndef __DXM_COM_FACTORY_H___
#define __DXM_COM_FACTORY_H___

#include "dxm_com_interface.h"

namespace dxm{
	namespace detail{
		class CComFactory{
		public:
			virtual ICom::Ptr Create()=0;
			virtual void Destory(ICom::Ptr com)=0;
		};

		template <class T>
		class TComFactory: public CComFactory{
		public:
			virtual ICom::Ptr Create() { return new T; }
			virtual void Destory(ICom::Ptr com) { delete com; }
		};
	}
}

#endif
#ifndef HTTP_SERVICE_H
#define HTTP_SERVICE_H

#include "boost/shared_ptr.hpp"
#include "component_service/component_service.h"

namespace Foundation{

	namespace HttpService{

		class IHttpService : public IComponent
		{
		public:
			typedef boost::shared_ptr<IHttpService> Ptr;

			virtual ~IHttpService(void)	{}

			/*
			 *	»ñÈ¡URLÄÚÈÝ;
			 */
			virtual bool GetUrlData(const std::string& url, std::string& content) = 0;
		};
	}
}

#endif
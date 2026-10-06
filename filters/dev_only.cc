/**
 *
 *  dev_only.cc
 *
 */

#include "dev_only.h"

#include <drogon/HttpAppFramework.h>

using namespace drogon;

drogon::Task<HttpResponsePtr> dev_only::doFilter(const HttpRequestPtr &req)
{
	auto res = drogon::HttpResponse::newHttpResponse();
	
  if constexpr (ENABLE_DEBUG_SHOPFRONT)
			co_return nullptr;	

	res->setStatusCode(k403Forbidden);

	co_return res;
}

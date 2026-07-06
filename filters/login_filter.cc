/**
 *
 *  login_filter.cc
 *
 */

#include "login_filter.h"

#include <drogon/HttpAppFramework.h>
#include <drogon/nosql/RedisClient.h>
#include <drogon/nosql/RedisResult.h>

using namespace drogon;

drogon::Task<HttpResponsePtr> login_filter::doFilter(const HttpRequestPtr &req)
{
	auto res = drogon::HttpResponse::newHttpResponse();
	
	if(std::string cookie = req->getCookie("session_token"); !cookie.empty())
	{
		auto redis = drogon::app().getFastRedisClient("shopfront_cache");


		auto result = co_await redis->execCommandCoro("GET %s", cookie.c_str());

		if (result.type() == drogon::nosql::RedisResultType::kNil || result.type() == drogon::nosql::RedisResultType::kError)
		{
			drogon::Cookie authcookie("session_token", "invalid");
			authcookie.setPath("/");
			authcookie.setMaxAge(0);
			authcookie.setExpiresDate(trantor::Date(0));
			authcookie.setHttpOnly(true);
			authcookie.setSecure(true);	
			res->addCookie(authcookie);
		}
		else
		{
			co_return nullptr;
		}
	}

	res->setStatusCode(k302Found);
	res->addHeader("Location", "/login");

	co_return res;
}

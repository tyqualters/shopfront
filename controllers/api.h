#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

inline auto GetClient()
{
	return drogon::app().getFastDbClient("default");
}

inline auto GetRedis()
{
	return drogon::app().getFastRedisClient("shopfront_cache");
}

inline Json::Value JsonStandardOk(std::optional<std::string>&& message = std::nullopt)
{
	Json::Value ret;
	ret["result"] = "ok";
	if(message.has_value())
		ret["message"] = message.value(); 

	return ret;
}

inline Json::Value JsonStandardError(std::string&& message = "See internal service logs")
{
	Json::Value ret;
	ret["result"] = "nok";
	ret["message"] = message;
	
	return ret;
}

class api : public drogon::HttpController<api>
{
public:
	METHOD_LIST_BEGIN
	
	// API Details
	METHOD_ADD(api::JsonOk, "", Get);
	METHOD_ADD(api::JsonOk, "/", Get);

#ifdef ENABLE_DEBUG_SHOPFRONT
	// List Users
	METHOD_ADD(api::ListUsersDevOnly, "/list-users", Get, "login_filter");
	// To SHA-256
	METHOD_ADD(api::ToSha256, "/sha256?message={}", Get);
	// Who Am I?
	METHOD_ADD(api::Whoami, "/whoami", Get,"login_filter");
#endif

	// User Registration
	METHOD_ADD(api::CreateUser, "/register", Post,"no_login_filter");
	
	// User Authentication
	METHOD_ADD(api::AuthenticateUser, "/login", Post, "no_login_filter");

	// User Profiles
	//METHOD_ADD(api::GetUserDetails, "/user/{id}", Get);

	// User Signout
	METHOD_ADD(api::SignoutUser, "/logout", Get, "login_filter");

	// Update User, Shop, Product
	METHOD_ADD(api::JsonOk, "/update", Post);
	
	// Shop Details
	METHOD_ADD(api::JsonOk, "/shop", Get);
	
	// Product/Item Details
	METHOD_ADD(api::JsonOk, "/product", Get);
	
	// Order Details (Get), Place Order (Post)
	METHOD_ADD(api::JsonOk, "/order", Get, Post);

	METHOD_LIST_END
	
	// Normal routes
	void JsonOk(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback);
	drogon::Task<HttpResponsePtr> CreateUser(HttpRequestPtr req);
	drogon::Task<HttpResponsePtr> AuthenticateUser(HttpRequestPtr req);	
	drogon::Task<HttpResponsePtr> SignoutUser(HttpRequestPtr req);
	drogon::Task<HttpResponsePtr> GetUserDetails(HttpRequestPtr req, std::string userId);

	// Dev only routes
	drogon::Task<HttpResponsePtr> Whoami(HttpRequestPtr req);
	drogon::Task<HttpResponsePtr> ListUsersDevOnly(HttpRequestPtr req);
	void ToSha256(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, std::string message);
};

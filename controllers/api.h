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
	METHOD_ADD(api::ListUsersDevOnly, "/list-users", Get, "dev_only");
	METHOD_ADD(api::ListShopsDevOnly, "/list-shops", Get, "dev_only");
	METHOD_ADD(api::ListProductsDevOnly, "/list-products", Get, "dev_only");
	METHOD_ADD(api::ToSha256, "/sha256?message={}", Get, "dev_only");
#endif
	// Who Am I?
	METHOD_ADD(api::Whoami, "/whoami", Get,"login_filter");

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
  METHOD_ADD(api::CreateShop, "/register-shop", Post);
  METHOD_ADD(api::GetShops, "/my-shops", Get, "login_filter");
	
	// Product/Item Details
	METHOD_ADD(api::CreateProduct, "/register-product", Post, "login_filter");
  METHOD_ADD(api::GetProduct, "/products", Get);
	
	// Order Details (Get), Place Order (Post)
	METHOD_ADD(api::JsonOk, "/order", Get, Post);

	METHOD_LIST_END
	
	// Normal routes
	void JsonOk(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback);
	drogon::Task<HttpResponsePtr> CreateUser(HttpRequestPtr req);
	drogon::Task<HttpResponsePtr> AuthenticateUser(HttpRequestPtr req);	
	drogon::Task<HttpResponsePtr> SignoutUser(HttpRequestPtr req);
	drogon::Task<HttpResponsePtr> GetUserDetails(HttpRequestPtr req, std::string userId);
  drogon::Task<HttpResponsePtr> CreateShop(HttpRequestPtr req);
  drogon::Task<HttpResponsePtr> GetShops(HttpRequestPtr req);
  drogon::Task<HttpResponsePtr> CreateProduct(HttpRequestPtr req);
  drogon::Task<HttpResponsePtr> GetProduct(HttpRequestPtr req);
	drogon::Task<HttpResponsePtr> Whoami(HttpRequestPtr req);
	
  // Dev only routes
	drogon::Task<HttpResponsePtr> ListUsersDevOnly(HttpRequestPtr req);
	drogon::Task<HttpResponsePtr> ListShopsDevOnly(HttpRequestPtr req);
	drogon::Task<HttpResponsePtr> ListProductsDevOnly(HttpRequestPtr req);
	void ToSha256(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, std::string message);
};

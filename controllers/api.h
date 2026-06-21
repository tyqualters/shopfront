#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class api : public drogon::HttpController<api>
{
public:
	METHOD_LIST_BEGIN
	
	// API Details
	METHOD_ADD(api::JsonOk, "", Get);
	METHOD_ADD(api::JsonOk, "/", Get);

	// User Registration
	METHOD_ADD(api::JsonOk, "/register", Get, Post);
	
	// User Authentication
	METHOD_ADD(api::JsonOk, "/login", Get, Post);
	
	// Update User, Shop, Product
	METHOD_ADD(api::JsonOk, "/update", Post);
	
	// Shop Details
	METHOD_ADD(api::JsonOk, "/shop", Get);
	
	// Product/Item Details
	METHOD_ADD(api::JsonOk, "/product", Get);
	
	// Order Details (Get), Place Order (Post)
	METHOD_ADD(api::JsonOk, "/order", Get, Post);

	METHOD_LIST_END
	
	void JsonOk(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);

};

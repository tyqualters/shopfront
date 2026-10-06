#include "api.h"
#include "Users.h"
#include "Shops.h"
#include "Products.h"

std::string ConvertSha256(std::string message);

// --------------------------------------------------------------
// 
// 	ROUTE METHODS
// 
// --------------------------------------------------------------

/**
 * Get a list of all current users
 */
drogon::Task<HttpResponsePtr> api::ListUsersDevOnly(HttpRequestPtr req)
{
	using namespace drogon_model::shopfront_db;

  // Mariadb Client
	auto client = GetClient();

  // Users ORM
	drogon::orm::CoroMapper<Users> mp(client);

	auto ret = JsonStandardOk();
	
  try 
	{
    // Get all Users
		std::vector<Users> users = co_await mp.findAll();

		// Loop through all users
		Json::Value usersArray(Json::arrayValue);
		for (const auto &user : users)
		{
			// append user
			usersArray.append(user.getValueOfUsername());
		}
		ret["users"] = usersArray;

		co_return drogon::HttpResponse::newHttpJsonResponse(ret);
	}
	catch (const drogon::orm::DrogonDbException &e)
	{
    // Database Error
		LOG_ERROR << "Error: " << e.base().what();

		co_return drogon::HttpResponse::newHttpJsonResponse(
			JsonStandardError()
		);
	}
}

/**
 * Get a list of all current shops 
 */
drogon::Task<HttpResponsePtr> api::ListShopsDevOnly(HttpRequestPtr req)
{
	using namespace drogon_model::shopfront_db;

  // Mariadb Client
	auto client = GetClient();

  // Users ORM
	drogon::orm::CoroMapper<Shops> mp(client);

	auto ret = JsonStandardOk();
	
  try 
	{
    // Get all Shops
		std::vector<Shops> shops = co_await mp.findAll();

		// Loop through all shops
		Json::Value shopsArray(Json::arrayValue);
		for (const auto &shop : shops)
		{
			// append shop
			shopsArray.append(shop.getValueOfShopname());
		}
		ret["shops"] = shopsArray;

		co_return drogon::HttpResponse::newHttpJsonResponse(ret);
	}
	catch (const drogon::orm::DrogonDbException &e)
	{
    // Database Error
		LOG_ERROR << "Error: " << e.base().what();

		co_return drogon::HttpResponse::newHttpJsonResponse(
			JsonStandardError()
		);
	}
}

/**
 * Get a list of all current products 
 */
drogon::Task<HttpResponsePtr> api::ListProductsDevOnly(HttpRequestPtr req)
{
	using namespace drogon_model::shopfront_db;

  // Mariadb Client
	auto client = GetClient();

  // Users ORM
	drogon::orm::CoroMapper<Products> mp(client);

	auto ret = JsonStandardOk();
	
  try 
	{
    // Get all Products 
		std::vector<Products> products = co_await mp.findAll();

		// Loop through all products
		Json::Value productsArray(Json::arrayValue);
		for (const auto &product : products)
		{
			// append product
			productsArray.append(product.getValueOfProductname());
		}
		ret["products"] = productsArray;

		co_return drogon::HttpResponse::newHttpJsonResponse(ret);
	}
	catch (const drogon::orm::DrogonDbException &e)
	{
    // Database Error
		LOG_ERROR << "Error: " << e.base().what();

		co_return drogon::HttpResponse::newHttpJsonResponse(
			JsonStandardError()
		);
	}
}

/**
 * Convert message to SHA-256
 */
void api::ToSha256(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, std::string message)
{	
	std::string m;
	if (!message.empty())
	{
		m = ConvertSha256(message);
	}
	else
	{
		m = "No string provided. Pass ?message= GET parameter.";
	}

	auto resp = HttpResponse::newHttpJsonResponse(JsonStandardOk(m));
	callback(resp);
}


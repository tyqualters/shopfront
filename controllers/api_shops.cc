#include "api.h"
#include "Shops.h"

#include <regex>
#include <optional>
#include <memory>


drogon::Task<std::optional<int32_t>> GetUserId(std::string&& token);



drogon::Task<HttpResponsePtr> api::GetShops(HttpRequestPtr req)
{
	using namespace drogon_model::shopfront_db;

  try {

    auto redis = GetRedis();
    auto client = GetClient();

    std::string token = req->getCookie("session_token"); 
    std::optional<int32_t> uid = co_await GetUserId(std::move(token));
		

    if(!uid.has_value())
      throw std::runtime_error("No valid session");
    

    // Shop name
    drogon::orm::CoroMapper<Shops> mp(client);
   
    std::vector<Shops> shops = co_await mp.findBy({Shops::Cols::_ownerId, orm::CompareOperator::EQ, uid.value()});
    
		auto ret = JsonStandardOk();

		// Loop through all shops 
		Json::Value shopsArray(Json::arrayValue);
		for (const auto &shop : shops)
		{
      Json::Value _shop;
      _shop["id"] = shop.getValueOfShopid();
      _shop["name"] = shop.getValueOfShopname();
      _shop["oid"] = shop.getValueOfOwnerid();
			shopsArray.append(_shop);
		}
		ret["shops"] = shopsArray;

    co_return drogon::HttpResponse::newHttpJsonResponse(
      ret
    );
  }
	catch (const drogon::orm::DrogonDbException &e)
	{
		LOG_ERROR << "Error: " << e.base().what();

		co_return drogon::HttpResponse::newHttpJsonResponse(
			JsonStandardError()
		);
	}
	catch (const std::exception &e)
	{	
		co_return drogon::HttpResponse::newHttpJsonResponse(
			JsonStandardError(e.what())	
		);
	}
  
}

/**
 * Note: uid is generated based on token. If no token exists, the filter should redirect.
 * This should ideally also be checked on client side.
 */
drogon::Task<HttpResponsePtr> api::CreateShop(HttpRequestPtr req)
{

	using namespace drogon_model::shopfront_db;

  try {

    auto redis = GetRedis();
    auto client = GetClient();

    std::string token = req->getCookie("session_token"); 
    std::optional<int32_t> uid = co_await GetUserId(std::move(token));
		

    if(!uid.has_value())
      throw std::runtime_error("No valid session");
    

    // Shop name
    const std::string &shopname = req->getParameter("shopname");

    drogon::orm::CoroMapper<Shops> mp(client);
    
    std::regex shopNamePattern(R"(^[a-zA-Z0-9._]+$)");	

      if (shopname.empty() || shopname.size() < 4) 
        throw std::invalid_argument("Shop name too short or empty");

      if (!std::regex_match(shopname, shopNamePattern))
        throw std::invalid_argument("Illegal characters in shop name");

      if (shopname.find(' ') != std::string::npos)
        throw std::invalid_argument("No whitespaces allowed in shopname");

    Shops shop;
    shop.setShopname(shopname);
    shop.setOwnerid(uid.value());

    co_await mp.insert(shop);
    
    co_return drogon::HttpResponse::newHttpJsonResponse(
      JsonStandardOk("Shop created")	
    );
  }
	catch (const drogon::orm::DrogonDbException &e)
	{
		LOG_ERROR << "Error: " << e.base().what();

		co_return drogon::HttpResponse::newHttpJsonResponse(
			JsonStandardError()
		);
	}
	catch (const std::exception &e)
	{	
		co_return drogon::HttpResponse::newHttpJsonResponse(
			JsonStandardError(e.what())	
		);
	}
}

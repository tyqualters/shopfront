#include "api.h"
#include "Shops.h"
#include "Products.h"

#include <regex>


drogon::Task<std::optional<int32_t>> GetUserId(std::string&& token);

/**
 * Note: uid is generated based on token. If no token exists, the filter should redirect.
 * This should ideally also be checked on client side.
 */
drogon::Task<HttpResponsePtr> api::CreateProduct(HttpRequestPtr req)
{

	using namespace drogon_model::shopfront_db;

  try {

    auto redis = GetRedis();
    auto client = GetClient();

    std::string token = req->getCookie("session_token"); 
    std::optional<int32_t> uid = co_await GetUserId(std::move(token));
		

    if(!uid.has_value())
      throw std::runtime_error("No valid session");
    

    // Parameters
    const std::string &productName = req->getParameter("product_name");
    float productPrice = std::stof(req->getParameter("product_price"));
    int32_t productShopId = std::stoi(req->getParameter("product_shopid"));

    drogon::orm::CoroMapper<Products> mp(client);
    drogon::orm::CoroMapper<Shops> mp2(client);
    
    std::regex productNamePattern(R"(^[a-zA-Z0-9._]+$)");	

      if (productName.empty() || productName.size() < 4) 
        throw std::invalid_argument("Shop name too short or empty");

      if (!std::regex_match(productName, productNamePattern))
        throw std::invalid_argument("Illegal characters in product name");

      if (productName.find(' ') != std::string::npos)
        throw std::invalid_argument("No whitespaces allowed in productName");

    // Verify the user owns the shop
    Shops shop = co_await mp2.findByPrimaryKey(productShopId);

    if(shop.getValueOfOwnerid() != uid.value())
      throw std::runtime_error("Insufficient permissions");

    Products product;
    product.setProductname(productName);
    product.setProductprice(std::to_string(productPrice));
    product.setShopid(productShopId);

    co_await mp.insert(product);
    
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

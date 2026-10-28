#include "api.h"
#include "Shops.h"
#include "Products.h"

#include <regex>


drogon::Task<std::optional<int32_t>> GetUserId(std::string&& token);

// --------------------------------------------------------------
// 
// 	ROUTE METHODS
// 
// --------------------------------------------------------------

/**
 * Create a new product
 *
 * Note: uid is generated based on token. If no token exists, the filter should redirect.
 * This should ideally also be checked on client side.
 */
drogon::Task<HttpResponsePtr> api::CreateProduct(HttpRequestPtr req)
{

	using namespace drogon_model::shopfront_db;

  // Get Mariadb client
  auto client = GetClient();

  // Temporary token
  std::string _token = req->getCookie("session_token"); 

  try {
    // Get user ID (moved _token)
    std::optional<int32_t> uid = co_await GetUserId(std::move(_token));
		
    // No uid = no valid session
    if(!uid.has_value())
      throw std::runtime_error("No valid session"); 

    // Parameters
    const std::string &productName = req->getParameter("product_name");
    float productPrice = std::stof(req->getParameter("product_price"));
    int32_t productShopId = std::stoi(req->getParameter("product_shopid"));

    // ORM for Products and Shops
    drogon::orm::CoroMapper<Products> mp(client);
    drogon::orm::CoroMapper<Shops> mp2(client);
    
    // Constraining parameter values
    std::regex productNamePattern(R"(^[a-zA-Z0-9._]+$)");	

    if (productName.empty() || productName.size() < 4) 
      throw std::invalid_argument("Shop name too short or empty");

    if (!std::regex_match(productName, productNamePattern))
      throw std::invalid_argument("Illegal characters in product name");

    if (productName.find(' ') != std::string::npos)
      throw std::invalid_argument("No whitespaces allowed in productName");

    // Verify the user owns the shop (permission check)
    Shops shop = co_await mp2.findByPrimaryKey(productShopId);

    // Not enough permissions
    if(shop.getValueOfOwnerid() != uid.value())
      throw std::runtime_error("Insufficient permissions");

    // Create the new product
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
    // Database error (Mariadb)
		LOG_ERROR << "Error: " << e.base().what();

		co_return drogon::HttpResponse::newHttpJsonResponse(
			JsonStandardError()
		);
	}
	catch (const std::exception &e)
	{
    // General exception
		co_return drogon::HttpResponse::newHttpJsonResponse(
			JsonStandardError(e.what())	
		);
	}
}

/*
 * Get products. Requires shopId. Optional productId. 
 **/
drogon::Task<HttpResponsePtr> api::GetProduct(HttpRequestPtr req)
{

	using namespace drogon_model::shopfront_db;
  using drogon::orm::Criteria;

  // Get Mariadb client
  auto client = GetClient();

  try {
    // Parameters
    const std::string &productId_s = req->getParameter("productid");
    const std::string &shopId_s = req->getParameter("shopid");
  
    if(shopId_s.empty())
    {
      co_return drogon::HttpResponse::newHttpJsonResponse(
          JsonStandardError("No shop ID specified")
          );
    }

    int32_t shopId = std::stoi(shopId_s), productId = !productId_s.empty() ? std::stoi(productId_s) : -1;

    // ORM for Products and Shops
    drogon::orm::CoroMapper<Products> mp(client);
    
    Json::Value ret = JsonStandardOk();

		Json::Value productsArray(Json::arrayValue);
    if (productId == -1)
    {
      std::vector<Products> products = co_await mp.findBy({Products::Cols::_shopId, orm::CompareOperator::EQ, shopId});
      for(auto& product: products)
      {
        productsArray.append(product.toJson());
      }
      // Return all products
    }
    else
    {
      std::vector<Products> products = co_await mp.findBy((Criteria(Products::Cols::_shopId, orm::CompareOperator::EQ, shopId)&&Criteria(Products::Cols::_productId, orm::CompareOperator::EQ, productId)));
      for(auto& product: products)
      {
        productsArray.append(product.toJson());
      }
      // Return a specific product
    }

    ret["products"] = productsArray;

    co_return drogon::HttpResponse::newHttpJsonResponse(
        ret
    );
  }
	catch (const drogon::orm::DrogonDbException &e)
	{
    // Database error (Mariadb)
		LOG_ERROR << "Error: " << e.base().what();

		co_return drogon::HttpResponse::newHttpJsonResponse(
			JsonStandardError()
		);
	}
	catch (const std::exception &e)
	{
    // General exception
		co_return drogon::HttpResponse::newHttpJsonResponse(
			JsonStandardError(e.what())	
		);
	}
}

#include "api.h"
#include "Users.h"

#include <regex>
#include <algorithm>
#include <cctype>

std::string ConvertSha256(std::string message);


drogon::Task<std::optional<int32_t>> GetUserId(std::string&& token);

// --------------------------------------------------------------
// 
// 	ROUTE METHODS
// 
// --------------------------------------------------------------

/**
 * Sign a user out 
 */
drogon::Task<HttpResponsePtr> api::SignoutUser(HttpRequestPtr req)
{
  // Redis Client
	auto redis = GetRedis();

  // Reset cookie
	drogon::Cookie authcookie("session_token", "invalid");
	authcookie.setPath("/");
	authcookie.setMaxAge(0);
	authcookie.setExpiresDate(trantor::Date(0));
	authcookie.setHttpOnly(true);
	authcookie.setSecure(true);

  // Reset cookie
  drogon::Cookie clientcookie("authid", "-1");
  clientcookie.setPath("/");
	clientcookie.setMaxAge(0);
	clientcookie.setExpiresDate(trantor::Date(0));
  clientcookie.setHttpOnly(false);
  clientcookie.setSecure(false);

  // If cookie existed, delete token from Redis
	if (std::string cookie = req->getCookie("session_token"); !cookie.empty())
	{
		auto result = co_await redis->execCommandCoro("UNLINK %s", cookie.c_str());
		if (result.type() == drogon::nosql::RedisResultType::kError)
			LOG_ERROR << "Something went wrong with deleting a token from Redis";
	}

	auto res = HttpResponse::newHttpResponse();

  // Redirect
	res->setStatusCode(k302Found);
	res->addHeader("Location", "/");

  // Update cookies
	res->addCookie(authcookie);
  res->addCookie(clientcookie);

	// TODO: Make this API response actually return a JSON value

	co_return res;
}

/**
 * Get user information from uid
 */
drogon::Task<HttpResponsePtr> api::Whoami(HttpRequestPtr req)
{
	using namespace drogon_model::shopfront_db;

  // Get Mariadb Client
	auto client = GetClient();

  // Token (temp)
	std::string _token = req->getCookie("session_token");

	try
	{
    // Get uid (moved _token)
    auto uid = co_await GetUserId(std::move(_token));

    // Not logged in
    if (!uid.has_value())
			throw std::runtime_error("No valid session.");

    // Users ORM
		drogon::orm::CoroMapper<Users> mp(client);

    // Find user by uid
		Users user = co_await mp.findOne({Users::Cols::_userId, orm::CompareOperator::EQ, uid.value()});

    // Return user information
		auto ret = JsonStandardOk("You are: " + user.getValueOfUsername());
    ret["uid"] = user.getValueOfUserid();
    std::string lcEmail = user.getValueOfUseremail();
    std::transform(lcEmail.begin(), lcEmail.end(), lcEmail.begin(), [](unsigned char c) { return std::tolower(c); });
    std::string lcEmailHash = ConvertSha256(lcEmail);
    std::transform(lcEmailHash.begin(), lcEmailHash.end(), lcEmailHash.begin(), [](unsigned char c) { return std::tolower(c); });
    ret["email"] = lcEmail;
    ret["gravatar"] = "https://www.gravatar.com/avatar/" + lcEmailHash + "?s=200&d=mp&r=pg";

		co_return drogon::HttpResponse::newHttpJsonResponse(
			ret	
		);
	}
	catch (const drogon::orm::DrogonDbException &e)
	{
    // Database error
		LOG_ERROR << e.base().what();

		co_return drogon::HttpResponse::newHttpJsonResponse(
			JsonStandardError()	
		);
	}
	catch (const std::exception &e)
	{
    // General exception
		LOG_ERROR << e.what();

		co_return drogon::HttpResponse::newHttpJsonResponse(
			JsonStandardError()
		);
	}
}

/**
 * Log a user in (<form>)
 */
drogon::Task<HttpResponsePtr> api::AuthenticateUser(HttpRequestPtr req)
{

	using namespace drogon_model::shopfront_db;

  // Get Mariadb Client
	auto client = GetClient();

  // Users ORM
	drogon::orm::CoroMapper<Users> mp(client);

	try
	{
    // get username and password params
		std::string username = req->getParameter("username"); 
		std::string password = req->getParameter("password"); 
		
		if (username.empty() || password.empty()) throw std::invalid_argument("Invalid User or Pass");
	
    // Find user by matching username
    Users user = co_await mp.findOne({Users::Cols::_userName, orm::CompareOperator::EQ, username});
	
    // Check password
		if (user.getValueOfUserpass() == password)
		{

			// TODO: Actual security
			// For now: Generate a UUID, store in Redis with expiration.
			// Every HTTP request with that UUID therefore = that user auth.
			
			// Generate token
			std::string uuid = drogon::utils::getUuid();

			// Set expiration time
			constexpr uint32_t expiration = 3600 * 24;

			// Get userId
			auto uid = user.getValueOfUserid();

			// Get Redis client
			auto redis = GetRedis();

			// TODO: Check? UUID shouldn't conflict but still.
			auto transaction = co_await redis->newTransactionCoro();
    			co_await transaction->execCommandCoro("SET %s %d EX %d", uuid.c_str(), uid, expiration);
   			co_await transaction->executeCoro();

			// Generate server-side cookie
			drogon::Cookie authcookie("session_token", uuid);
			authcookie.setPath("/");
			authcookie.setExpiresDate(trantor::Date::date().after(expiration));
			authcookie.setHttpOnly(true);
			authcookie.setSecure(true);

      // Generate client-side cookie
			drogon::Cookie clientcookie("authid", std::to_string(uid));
			clientcookie.setPath("/");
			clientcookie.setExpiresDate(trantor::Date::date().after(expiration));
			clientcookie.setHttpOnly(false);
			clientcookie.setSecure(false);

			// Create response
			auto ret = JsonStandardOk("Authentication successful.");
			ret["token"] = uuid;

			auto resp = drogon::HttpResponse::newHttpJsonResponse(
				ret	
			);

			resp->addCookie(authcookie);
      resp->addCookie(clientcookie);
      
      // Redirect
      resp->setStatusCode(k302Found);
      resp->addHeader("Location", "/");

			co_return resp;
		} else throw std::invalid_argument("Invalid User or Pass");
	}
	catch(...)
	{
    // Catch-All exceptions
		co_return drogon::HttpResponse::newHttpJsonResponse(
			JsonStandardError("Username or Password invalid")	
		);
	}
}

/**
 * Create a new user
 */
drogon::Task<HttpResponsePtr> api::CreateUser(HttpRequestPtr req)
{
	using namespace drogon_model::shopfront_db;

  // Mariadb Client
	auto client = GetClient();

  // Users ORM
	drogon::orm::CoroMapper<Users> mp(client);

  // Regex for constraints
	std::regex emailPattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
	std::regex userNamePattern(R"(^[a-zA-Z0-9._]+$)");	
	
	try
	{
		// Create User 
		Users user;
	
    // Get parameters
		const std::string &username = req->getParameter("username");
		const std::string &email = req->getParameter("email");
		const std::string &password = req->getParameter("password");
		
		// Constraints:
		// 	Username: Unique, not empty, only ASCII, no spaces
		// 	Password: Min 12 characters, only ASCII, no spaces
		// 	Email: C/W RFC 5322

		if (username.empty() || username.size() < 4) 
			throw std::invalid_argument("Username too short or empty");

		if (!std::regex_match(username, userNamePattern))
			throw std::invalid_argument("Illegal characters in username");

		if (username.find(' ') != std::string::npos)
			throw std::invalid_argument("No whitespaces allowed in username");
		
		if (email.empty())
			throw std::invalid_argument("Email empty");

		
		if (!std::regex_match(email, emailPattern))
			throw std::invalid_argument("Email does not conform to RFC 5322 standards");

		if (password.empty() || password.size() < 12)
			throw std::invalid_argument("Password must be at least 12 characters long");

		if (password.find(' ') != std::string::npos)
			throw std::invalid_argument("No whitespaces allowed in password");

    // Update user information
		user.setUsername(username);
		user.setUseremail(email);
		user.setUserpass(password);
		
    // Push to database
		co_await mp.insert(user);

		co_return drogon::HttpResponse::newHttpJsonResponse(
			JsonStandardOk("User created")	
		);
	}
	catch (const drogon::orm::DrogonDbException &e)
	{
    // Database error
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



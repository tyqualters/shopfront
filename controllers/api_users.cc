#include "api.h"
#include "Users.h"

#include <regex>

// --------------------------------------------------------------
// 
// 	ROUTE METHODS
// 
// --------------------------------------------------------------

#if 0
drogon::Task<HttpResponsePtr> api::GetUserDetails(HttpRequestPtr req, std::string userId)
{
	if (userId.empty())
	{
		co_return drogon::HttpResponse::newHttpJsonResponse(
			JsonStandardError("No user id specified")	
		);
	}

	if (userId == "me")
	{
		// Pull detailed report
		if (std::string cookie = req->getCookie("session_token"); !cookie.empty())
		{
			auto redis = GetRedis();
			auto res = co_await redis->execCommandCoro("GET %s", cookie.c_str());
			if (res.type() == drogon::nosql::RedisResultType::kNil || res.type() == drogon::nosql::RedisResultType::kError)
			{
				co_return drogon::HttpResponse::newHttpJsonResponse(
					JsonStandardError()	
				);
			}
			
			userId = res.asString();

			// Make privileged request
		}
		else
		{
			co_return drogon::HttpResponse::newHttpJsonResponse(
				JsonStandardError("No user id specified")	
			);
		}
	}
	else 
	{
		// Make non-privileged request
	}
}
#endif

drogon::Task<HttpResponsePtr> api::SignoutUser(HttpRequestPtr req)
{
	auto redis = GetRedis();

	drogon::Cookie authcookie("session_token", "invalid");
	authcookie.setPath("/");
	authcookie.setMaxAge(0);
	authcookie.setExpiresDate(trantor::Date(0));
	authcookie.setHttpOnly(true);
	authcookie.setSecure(true);

  drogon::Cookie clientcookie("authid", "-1");
  clientcookie.setPath("/");
	clientcookie.setMaxAge(0);
	clientcookie.setExpiresDate(trantor::Date(0));
  clientcookie.setHttpOnly(false);
  clientcookie.setSecure(false);


	if (std::string cookie = req->getCookie("session_token"); !cookie.empty())
	{
		auto result = co_await redis->execCommandCoro("UNLINK %s", cookie.c_str());
		if (result.type() == drogon::nosql::RedisResultType::kError)
			LOG_ERROR << "Something went wrong with deleting a token from Redis";
	}

	auto res = HttpResponse::newHttpResponse();

	res->setStatusCode(k302Found);
	res->addHeader("Location", "/");

	res->addCookie(authcookie);
  res->addCookie(clientcookie);

	// Todo: Make this API response actually return a JSON value

	co_return res;
}

drogon::Task<HttpResponsePtr> api::Whoami(HttpRequestPtr req)
{
	using namespace drogon_model::shopfront_db;

	auto redis = GetRedis();

	auto client = GetClient();

	try
	{
		std::string token = req->getCookie("session_token");

		if (token.empty())
			throw std::runtime_error("No token provided");

		auto transaction = co_await redis->newTransactionCoro();
		co_await transaction->execCommandCoro("GET %s", token.c_str());
		auto res = co_await transaction->executeCoro();
		
		std::vector<drogon::nosql::RedisResult> redisResults;

		if (res.type() != drogon::nosql::RedisResultType::kArray || (redisResults = res.asArray()).size() == 0)
			throw std::runtime_error("Transaction failed or returned invalid format");

		if (redisResults[0].type() == drogon::nosql::RedisResultType::kNil || redisResults[0].type() == drogon::nosql::RedisResultType::kError)
			throw std::runtime_error("Token not found");

		auto uid = redisResults[0].asString();

		LOG_DEBUG << "Whoami Uid: " << uid;

		drogon::orm::CoroMapper<Users> mp(client);

		Users user = co_await mp.findOne({Users::Cols::_userId, orm::CompareOperator::EQ, uid});

		auto ret = JsonStandardOk("You are: " + user.getValueOfUsername());
		ret["token"] = token;

		co_return drogon::HttpResponse::newHttpJsonResponse(
			ret	
		);
	}
	catch (const drogon::orm::DrogonDbException &e)
	{
		LOG_ERROR << e.base().what();

		co_return drogon::HttpResponse::newHttpJsonResponse(
			JsonStandardError()	
		);
	}
	catch (const std::exception &e)
	{
		LOG_ERROR << e.what();

		co_return drogon::HttpResponse::newHttpJsonResponse(
			JsonStandardError()
		);
	}
}

drogon::Task<HttpResponsePtr> api::AuthenticateUser(HttpRequestPtr req)
{

	using namespace drogon_model::shopfront_db;

	auto client = GetClient();

	drogon::orm::CoroMapper<Users> mp(client);

	try
	{
		std::string username = req->getParameter("username"); 
		std::string password = req->getParameter("password"); 
		
		if (username.empty() || password.empty()) throw std::invalid_argument("Invalid User or Pass");
		
		Users user = co_await mp.findOne({Users::Cols::_userName, orm::CompareOperator::EQ, username});
		
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
      

      resp->setStatusCode(k302Found);
      resp->addHeader("Location", "/");

			co_return resp;
		} else throw std::invalid_argument("Invalid User or Pass");
	}
	catch(...)
	{
		co_return drogon::HttpResponse::newHttpJsonResponse(
			JsonStandardError("Username or Password invalid")	
		);
	}
}

drogon::Task<HttpResponsePtr> api::CreateUser(HttpRequestPtr req)
{
	using namespace drogon_model::shopfront_db;

	auto client = GetClient();

	drogon::orm::CoroMapper<Users> mp(client);


	std::regex emailPattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
	std::regex userNamePattern(R"(^[a-zA-Z0-9._]+$)");	
	
	try
	{
		// Do sanity checks
		Users user;
		
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

		user.setUsername(username);
		user.setUseremail(email);
		user.setUserpass(password);
		
		co_await mp.insert(user);

		co_return drogon::HttpResponse::newHttpJsonResponse(
			JsonStandardOk("User created")	
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

drogon::Task<HttpResponsePtr> api::ListUsersDevOnly(HttpRequestPtr req)
{
	using namespace drogon_model::shopfront_db;

	auto client = GetClient();

	// C++20 Coroutines
	drogon::orm::CoroMapper<Users> mp(client);

	try 
	{
        	// Pause coroutine until users acquired
		std::vector<Users> users = co_await mp.findAll();

		auto ret = JsonStandardOk();

		// Loop through all users
		Json::Value usersArray(Json::arrayValue);
		for (const auto &user : users)
		{
			// username is "not null"
			usersArray.append(user.getValueOfUsername());
		}
		ret["users"] = usersArray;

		// co_return is used instead of callback
		co_return drogon::HttpResponse::newHttpJsonResponse(ret);
	}
	catch (const drogon::orm::DrogonDbException &e)
	{
		LOG_ERROR << "Error: " << e.base().what();

		co_return drogon::HttpResponse::newHttpJsonResponse(
			JsonStandardError()
		);
	}
}


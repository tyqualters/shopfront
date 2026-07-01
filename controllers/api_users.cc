#include "api.h"
#include "Users.h"

#include <regex>

// --------------------------------------------------------------
// 
// 	ROUTE METHODS
// 
// --------------------------------------------------------------

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
			Json::Value ret;
			ret["result"] = "ok";
			ret["message"] = "Authentication successful.";

			co_return drogon::HttpResponse::newHttpJsonResponse(
				ret	
			);
		} else throw std::invalid_argument("Invalid User or Pass");
	}
	catch (const drogon::orm::DrogonDbException &e)
	{
		LOG_ERROR << "Error: " << e.base().what();

		co_return drogon::HttpResponse::newHttpJsonResponse(
			JsonStandardError()
		);
	}
	catch(...)
	{

		Json::Value ret;
		ret["result"] = "nok";
		ret["message"] = "Username or Password invalid";

		co_return drogon::HttpResponse::newHttpJsonResponse(
			ret	
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

		Json::Value ret;
		ret["result"] = "ok";
		ret["message"] = "User created";

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
		Json::Value ret;
		ret["result"] = "nok";
		ret["message"] = e.what();

		co_return drogon::HttpResponse::newHttpJsonResponse(
			ret	
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

		Json::Value ret;
		ret["result"] = "ok";

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


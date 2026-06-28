#include <openssl/evp.h>
#include <regex>

#include "api.h"
#include "Users.h"

// --------------------------------------------------------------
// 
// 	UTILITY FUNCTIONS
// 
// --------------------------------------------------------------

inline auto GetClient()
{
	return drogon::app().getFastDbClient("default");
}


template<typename... Args>
inline void ExecSql(std::string, std::function<void (const drogon::orm::Result &, Json::Value &)> &&, std::function<void (const HttpResponsePtr &)> &&, Args &&...);

const Json::Value JsonStandardError()
{
	Json::Value ret;
	ret["result"] = "nok";
	ret["message"] = "See internal service logs";
	
	return ret;
}

std::string ConvertSha256(std::string message)
{
	// https://docs.openssl.org/3.2/man3/SHA256_Init/#synopsis
	// Changes from legacy code
	// Use EVP_DigestInit_ex, EVP_DigestUpdate, and EVP_DigestFinal_ex
	
	static const char* hexCodes = "0123456789ABCDEF";

	EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
	const EVP_MD *md = EVP_sha256();

	unsigned char md_value[EVP_MAX_MD_SIZE];
	unsigned int md_len;

	if (!EVP_DigestInit_ex(mdctx, md, nullptr))
	{
		LOG_ERROR << "Error: OpenSSL message digest initialization failed";
		EVP_MD_CTX_free(mdctx);
		return std::string();
	}

	if (!EVP_DigestUpdate(mdctx, message.c_str(), message.size()))
	{
		LOG_ERROR << "Error: OpenSSL message digest update failed";
		EVP_MD_CTX_free(mdctx);
		return std::string();
	}

	if (!EVP_DigestFinal_ex(mdctx, md_value, &md_len))
	{
		LOG_ERROR << "Error: OpenSSL message digest finalization failed";
		EVP_MD_CTX_free(mdctx);
		return std::string();
	}

	EVP_MD_CTX_free(mdctx);

	std::string hash;
	hash.reserve((md_len * 2) + 1);

	for(int i = 0; i < md_len; ++i)
	{
		unsigned char byte = md_value[i]; 

		char bytes[2];
		bytes[0] = hexCodes[byte / 16];
		bytes[1] = hexCodes[byte % 16];

		hash += bytes[0]; 
		hash += bytes[1];
	}
	
	return hash;
}

// --------------------------------------------------------------
// 
// 	ROUTE METHODS
// 
// --------------------------------------------------------------

void api::JsonOk(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback)
{
	Json::Value ret;
	ret["result"] = "ok";
	auto resp = HttpResponse::newHttpJsonResponse(ret);
	callback(resp);
}

void api::ToSha256(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, std::string message)
{
	Json::Value ret;

	if (!message.empty())
	{
		ret["result"] = "ok";
		ret["message"] = ConvertSha256(message);
	}
	else
	{
		ret["result"] = "nok";
		ret["message"] = "No string provided. Pass ?message= GET parameter.";
	}

	auto resp = HttpResponse::newHttpJsonResponse(ret);
	callback(resp);
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

// --------------------------------------------------------------
// 
// 	OLD CALLBACK VARIANTS JUST FOR FUTURE REFERENCE
// 
// --------------------------------------------------------------

// Run SQL statements (Advised to use models first!)
template<typename... Args>
inline void ExecSql(std::string query, std::function<void (const drogon::orm::Result &, Json::Value &)> &&fn, std::function<void (const HttpResponsePtr &)> &&callback, Args &&... args)
{
	auto client = GetClient();
	client->execSqlAsync(query, 
		[callback, fn](const drogon::orm::Result &result)
		{
			Json::Value ret;
			ret["result"] = "ok";
				
			fn(result, ret);
			
			auto resp = HttpResponse::newHttpJsonResponse(ret);
			callback(resp);
		}, 
		[callback](const drogon::orm::DrogonDbException &e)
		{
			std::cerr << "Error: " << e.base().what() << std::endl;
			Json::Value ret;
			ret["result"] = "nok";
			ret["message"] = "See internal service logs";
			auto resp = HttpResponse::newHttpJsonResponse(ret);
			callback(resp);
		},
		std::forward<Args>(args)...);
}

// (Callback Version)
//void api::CreateUser(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback)
//{
//	std::string username = req->getParameter("username");
//
//	if(username.empty())
//	{
//		Json::Value ret;
//		ret["result"] = "nok";
//		ret["message"] = "No username provided";
//		auto resp = HttpResponse::newHttpJsonResponse(ret);
//		callback(resp);
//	}
//	else
//	{
//		ExecSql("insert into users (username) values (?)", [username](const auto &result, auto &ret)
//				{
//					ret["message"] = "User created";
//					LOG_INFO << "User " << username << " created.";
//				},
//				std::move(callback),
//				username
//		       );
//	}
//}

// (Callback Version)
//void api::ListUsersDevOnly(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback)
//{
//	ExecSql("select * from users", [](const auto &result, auto &ret)
//		{
//			Json::Value usersArray(Json::arrayValue);
//			for(auto row : result)
//			{
//				usersArray.append(row["username"].template as<std::string>());
//			}
//
//			ret["users"] = usersArray;
//		},
//		std::move(callback)
//	);
//}

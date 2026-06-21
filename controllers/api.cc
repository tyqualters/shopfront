#include "api.h"

inline auto GetClient()
{
	return drogon::app().getFastDbClient("default");
}

inline void ExecSql(std::string query, std::function<void (const drogon::orm::Result &, Json::Value &)> &&fn, std::function<void (const HttpResponsePtr &)> &&callback)
{
	auto dbClient = GetClient();
	dbClient->execSqlAsync(query, 
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
		});
}

void api::JsonOk(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback)
{
	Json::Value ret;
	ret["result"] = "ok";
	auto resp = HttpResponse::newHttpJsonResponse(ret);
	callback(resp);
}

void CreateUser(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, std::string username)
{

}

void api::ListUsersDevOnly(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback)
{
	ExecSql("select * from users", [](const auto &result, auto &ret)
		{
			Json::Value usersArray(Json::arrayValue);
			for(auto row : result)
			{
				usersArray.append(row["username"].template as<std::string>());
			}

			ret["users"] = usersArray;
		},
		std::move(callback)
	);
}

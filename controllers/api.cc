#include "api.h"

void api::JsonOk(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback)
{
	Json::Value ret;
	ret["result"] = "ok";
	auto resp = HttpResponse::newHttpJsonResponse(ret);
	callback(resp);
}

void api::ListUsersDevOnly(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback)
{
	auto dbClient = drogon::app().getFastDbClient("default");
	dbClient->execSqlAsync("select * from users", [callback](const drogon::orm::Result &result)
			{
				Json::Value ret;
				ret["result"] = "ok";

				Json::Value usersArray(Json::arrayValue);
				for(auto row : result)
				{
					usersArray.append(row["username"].as<std::string>());
				}

				ret["users"] = usersArray;
				
				auto resp = HttpResponse::newHttpJsonResponse(ret);
				callback(resp);
			},
			[callback](const drogon::orm::DrogonDbException &e)
			{
				std::cerr << "Error: " << e.base().what() << std::endl;
				Json::Value ret;
				ret["result"] = "nok";
				auto resp = HttpResponse::newHttpJsonResponse(ret);
				callback(resp);
			});
}

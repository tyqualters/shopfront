#include "api.h"

void api::JsonOk(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback)
{
	Json::Value ret;
	ret["result"] = "ok";
	auto resp = HttpResponse::newHttpJsonResponse(ret);
	callback(resp);
}



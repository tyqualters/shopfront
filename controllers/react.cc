#include "react.h"


void react::asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
	auto resp = drogon::HttpResponse::newFileResponse("./public_html/index.html");
	resp->setContentTypeCode(drogon::CT_TEXT_HTML);
	resp->setStatusCode(k200OK);
	callback(resp);
}

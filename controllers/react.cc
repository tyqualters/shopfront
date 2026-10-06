#include "react.h"

/**
 * Send index page (SimpleHttpController)
 */
void react::asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
	auto resp = drogon::HttpResponse::newFileResponse("./public_html/index.html");
	resp->setContentTypeCode(drogon::CT_TEXT_HTML);
	resp->setStatusCode(k200OK);
	callback(resp);
}

/**
 * Send index page (HttpController)
 */
drogon::Task<HttpResponsePtr> reactregex::SendIndex(HttpRequestPtr req)
{
	auto resp = drogon::HttpResponse::newFileResponse("./public_html/index.html");
	resp->setContentTypeCode(drogon::CT_TEXT_HTML);
	resp->setStatusCode(k200OK);
	co_return resp;
}

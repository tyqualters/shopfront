#pragma once

#include <drogon/HttpSimpleController.h>

using namespace drogon;

// Basically everything here needs to navigate back to the index.html page
//  because React uses Client-side Rendering (CSR) instead of Static Site
//  Generation (SSG) like Next.js does.

class react : public drogon::HttpSimpleController<react>
{
public:
	void asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback) override;
	PATH_LIST_BEGIN
	// list path definitions here;
	// PATH_ADD("/path", "filter1", "filter2", HttpMethod1, HttpMethod2...);
	PATH_ADD("/login", Get, "no_login_filter");
	PATH_ADD("/register", Get, "no_login_filter");
	PATH_ADD("/test", Get, "login_filter");
	PATH_LIST_END
};

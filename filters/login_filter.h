/**
 *
 *  login_filter.h
 *
 */

#pragma once

#include <drogon/HttpFilter.h>
using namespace drogon;


class login_filter : public HttpCoroFilter<login_filter>
{
public:
	drogon::Task<HttpResponsePtr> doFilter(const HttpRequestPtr &req) override;
};


/**
 *
 *  no_login_filter.h
 *
 */

#pragma once

#include <drogon/HttpFilter.h>
using namespace drogon;


class no_login_filter : public HttpCoroFilter<no_login_filter>
{
  public:
	drogon::Task<HttpResponsePtr> doFilter(const HttpRequestPtr &req) override;
};


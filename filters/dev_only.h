/**
 *
 *  dev_only.h
 *
 */

#pragma once

#include <drogon/HttpFilter.h>
using namespace drogon;


class dev_only : public HttpCoroFilter<dev_only>
{
public:
	drogon::Task<HttpResponsePtr> doFilter(const HttpRequestPtr &req) override;
};


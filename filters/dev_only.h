/**
 *
 *  dev_only.h
 *
 */

#pragma once

#include <drogon/HttpFilter.h>
using namespace drogon;

#ifndef ENABLE_DEBUG_SHOPFRONT
#define ENABLE_DEBUG_SHOPFRONT 0
#endif

class dev_only : public HttpCoroFilter<dev_only>
{
public:
	drogon::Task<HttpResponsePtr> doFilter(const HttpRequestPtr &req) override;
};


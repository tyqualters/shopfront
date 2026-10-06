#include "api.h"

// --------------------------------------------------------------
// 
// 	ROUTE METHODS
// 
// --------------------------------------------------------------

/**
 * Standard Json OK response
 */
void api::JsonOk(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback)
{
  // Note: JsonStandardOk() is an inline function in header
	auto resp = HttpResponse::newHttpJsonResponse(JsonStandardOk());
	callback(resp);
}


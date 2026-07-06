#include "api.h"

#include <jwt-cpp/jwt.h>

// --------------------------------------------------------------
// 
// 	AUTHENTICATION FUNCTIONS
// 
// --------------------------------------------------------------

drogon::Task<bool> CreateJWT()
{
	auto token = jwt::create()
		.set_type("JWS");
	co_return true;
}

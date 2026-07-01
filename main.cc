#include <drogon/drogon.h>

int main(int argc, char** argv)
{
	// Set HTTP listener address and port
	// drogon::app().addListener("0.0.0.0", port);
	// Load config file
	drogon::app().loadConfigFile("../config.yaml");

	auto& config = drogon::app().getCustomConfig();
	// TODO: Parse a .env file here if present, also look for std::env vars
	// TODO: Print all port numbers

	// Run HTTP framework,the method will block in the internal event loop
	LOG_INFO << "Shopfront starting";/* on port " << port;*/
#ifdef ENABLE_DEBUG_SHOPFRONT
	LOG_WARN << "DEBUG MODE ENABLED";
#endif
	drogon::app().run();

	return EXIT_SUCCESS;
}

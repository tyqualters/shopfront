#include <drogon/drogon.h>
#include <cstring>

int main(int argc, char** argv)
{

	std::string configFile = "config.yaml";

	for(int i = 0; i < argc; ++i)
	{
		if (std::strcmp(argv[i], "-c") == 0 || std::strcmp(argv[i], "--config") == 0)
		{
			if (i + 1 < argc)
			{
				configFile = argv[++i];
				LOG_INFO << "Arg-Set Config File: " << configFile;
			}
		}
	}

	// Set HTTP listener address and port
	// drogon::app().addListener("0.0.0.0", port);
	// Load config file
	drogon::app().loadConfigFile(configFile);

	auto& config = drogon::app().getCustomConfig();
	// TODO: Parse a .env file here if present, also look for std::env vars
	// TODO: Print all port numbers

	// Run HTTP framework,the method will block in the internal event loop
	LOG_INFO << "Shopfront starting";/* on port " << port;*/
#ifdef ENABLE_DEBUG_SHOPFRONT
	trantor::Logger::setLogLevel(trantor::Logger::kDebug);
	LOG_WARN << "DEBUG MODE ENABLED";
#endif

	drogon::app().run();

	return EXIT_SUCCESS;
}

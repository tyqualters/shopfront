#include <drogon/drogon.h>
int main(int argc, char** argv)
{
	int port = 5555;
	for(int i = 1; i < argc; ++i)
	{
		if (std::strcmp(argv[i], "-p") == 0 && i + 1 < argc)
		{
			try
			{
				port = std::atoi(argv[++i]);
			} catch (const std::exception& e)
			{
				LOG_ERROR << "Could not parse port number";
			}
		}
	}

	// Set HTTP listener address and port
	drogon::app().addListener("0.0.0.0", port);
	// Load config file
	drogon::app().loadConfigFile("../config.yaml");
	// Run HTTP framework,the method will block in the internal event loop
	LOG_INFO << "Shopfront starting on port " << port;
	drogon::app().run();

	return EXIT_SUCCESS;
}

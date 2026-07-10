#include <drogon/drogon.h>
#include <yaml-cpp/yaml.h>
#include <json/json.h>
#include <cstring>
#include <fstream>
#include <regex>
#include <algorithm>
#include <cctype>

// yaml2cpp (lib/src/YamlConfigAdapter.cc)
// Only permitted because Drogon is published under MIT License
static bool yaml2json(const YAML::Node &node, Json::Value &jsonValue)
{
    if (node.IsNull())
    {
        return false;
    }
    else if (node.IsScalar())
    {
        if (node.Tag() != "!")
        {
            try
            {
                jsonValue = node.as<Json::Value::Int64>();
                return true;
            }
            catch (const YAML::BadConversion &e)
            {
            }
            try
            {
                jsonValue = node.as<double>();
                return true;
            }
            catch (const YAML::BadConversion &e)
            {
            }
            try
            {
                jsonValue = node.as<bool>();
                return true;
            }
            catch (const YAML::BadConversion &e)
            {
            }
        }

        Json::Value v(node.Scalar());
        jsonValue.swapPayload(v);
        return true;
    }
    else if (node.IsSequence())
    {
        for (std::size_t i = 0; i < node.size(); i++)
        {
            Json::Value v;
            if (yaml2json(node[i], v))
            {
                jsonValue.append(v);
            }
            else
            {
                return false;
            }
        }

        return true;
    }
    else if (node.IsMap())
    {
        for (YAML::const_iterator it = node.begin(); it != node.end(); ++it)
        {
            Json::Value v;
            if (yaml2json(it->second, v))
            {
                jsonValue[it->first.Scalar()] = v;
            }
            else
            {
                return false;
            }
        }

        return true;
    }

    return false;
}


void LoadConfig(std::string configFile)
{
	// Holy AI-Assist... Thank you, Gemini for saving me brain cells.
	
	static auto trim = [](std::string s) -> std::string
	{
	    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
	    }));
	    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
	    }).base(), s.end());
	    return s;
	};

	LOG_INFO << "Loading config file: " << configFile;

	// Open a file stream
	std::ifstream ifs(configFile.c_str());
	if (!ifs.is_open()) throw std::runtime_error("Failed to open config file");
	
	// Read file into string
	std::string yaml((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	// Regex match ${...:-...}
	std::regex envRegex(R"(\$\{((?:(?!:-)[^}])+)(?::-([^}]+))?\})");
    	std::smatch match;
	while (std::regex_search(yaml, match, envRegex))
	{
		// match[0] = ${VAR:-Default}, match[1] = VAR, match[2] = Default
		std::string envVarName = trim(match[1].str());
		const char* envValue = std::getenv(envVarName.c_str());
		std::string replacement{};

		if (envValue && *envValue != '\0')
		{
        		replacement = trim(envValue);
    		}
		else if (match[2].matched)
		{
        		replacement = trim(match[2].str());
    		}

		LOG_DEBUG << match[0].str() << " translating to " << replacement;
		yaml.replace(match.position(0), match.length(0), replacement);
    	}

	// Parse as YAML
	YAML::Node yamlConfig = YAML::Load(yaml);
	Json::Value jsonConfig;
	yaml2json(yamlConfig, jsonConfig);

	// Import jsonConfig
	drogon::app().loadConfigJson(jsonConfig);
}

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

	// Load config file
	LoadConfig(configFile);

	// auto& config = drogon::app().getCustomConfig();
	// TODO: Print all port numbers

	LOG_INFO << "Shopfront starting";/* on port " << port;*/
#ifdef ENABLE_DEBUG_SHOPFRONT
	trantor::Logger::setLogLevel(trantor::Logger::kDebug);
	LOG_WARN << "DEBUG MODE ENABLED";
#endif

	drogon::app().run();

	return EXIT_SUCCESS;
}

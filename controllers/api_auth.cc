#include "api.h"

#include <optional>

drogon::Task<std::optional<int32_t>> GetUserId(std::string&& token) {
  
	auto redis = GetRedis();
	auto client = GetClient();

	try
	{
		if (token.empty())
     co_return std::nullopt;

		auto transaction = co_await redis->newTransactionCoro();
		co_await transaction->execCommandCoro("GET %s", token.c_str());
		auto res = co_await transaction->executeCoro();
		
		std::vector<drogon::nosql::RedisResult> redisResults;

		if (res.type() != drogon::nosql::RedisResultType::kArray || (redisResults = res.asArray()).size() == 0)
			throw std::runtime_error("Transaction failed or returned invalid format");

		if (redisResults[0].type() == drogon::nosql::RedisResultType::kNil || redisResults[0].type() == drogon::nosql::RedisResultType::kError)
			co_return std::nullopt;

		auto uid = redisResults[0].asString();

    co_return std::stoi(uid);
	}
	catch (const drogon::orm::DrogonDbException &e)
	{
		LOG_ERROR << e.base().what();

		co_return std::nullopt;
	}
	catch (const std::exception &e)
	{
		LOG_ERROR << e.what();

		co_return std::nullopt;
	}
}

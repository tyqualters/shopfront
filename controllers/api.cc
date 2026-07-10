#include "api.h"

#include <expected>
#include <openssl/evp.h>


// --------------------------------------------------------------
// 
// 	UTILITY FUNCTIONS
// 
// --------------------------------------------------------------

std::string ConvertSha256(std::string message)
{
	// https://docs.openssl.org/3.2/man3/SHA256_Init/#synopsis
	// Changes from legacy code
	// Use EVP_DigestInit_ex, EVP_DigestUpdate, and EVP_DigestFinal_ex
	
	static const char* hexCodes = "0123456789ABCDEF";

	EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
	const EVP_MD *md = EVP_sha256();

	unsigned char md_value[EVP_MAX_MD_SIZE];
	unsigned int md_len;

	if (!EVP_DigestInit_ex(mdctx, md, nullptr))
	{
		LOG_ERROR << "Error: OpenSSL message digest initialization failed";
		EVP_MD_CTX_free(mdctx);
		return std::string();
	}

	if (!EVP_DigestUpdate(mdctx, message.c_str(), message.size()))
	{
		LOG_ERROR << "Error: OpenSSL message digest update failed";
		EVP_MD_CTX_free(mdctx);
		return std::string();
	}

	if (!EVP_DigestFinal_ex(mdctx, md_value, &md_len))
	{
		LOG_ERROR << "Error: OpenSSL message digest finalization failed";
		EVP_MD_CTX_free(mdctx);
		return std::string();
	}

	EVP_MD_CTX_free(mdctx);

	std::string hash;
	hash.reserve((md_len * 2) + 1);

	for(int i = 0; i < md_len; ++i)
	{
		unsigned char byte = md_value[i]; 

		char bytes[2];
		bytes[0] = hexCodes[byte / 16];
		bytes[1] = hexCodes[byte % 16];

		hash += bytes[0]; 
		hash += bytes[1];
	}
	
	return hash;
}

// --------------------------------------------------------------
// 
// 	ROUTE METHODS
// 
// --------------------------------------------------------------

void api::JsonOk(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback)
{
	auto resp = HttpResponse::newHttpJsonResponse(JsonStandardOk());
	callback(resp);
}

void api::ToSha256(const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback, std::string message)
{	
	std::string m;
	if (!message.empty())
	{
		m = ConvertSha256(message);
	}
	else
	{
		m = "No string provided. Pass ?message= GET parameter.";
	}

	auto resp = HttpResponse::newHttpJsonResponse(JsonStandardOk(m));
	callback(resp);
}


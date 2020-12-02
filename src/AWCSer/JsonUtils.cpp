#include <AWCSer/JsonUtils.h>

bool JsonUtils::HasKey(const Json& json, const std::string& key)
{
	bool hasKey = json.find(key) != json.end();
	return hasKey;
}
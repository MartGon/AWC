#include <AWCSer/JsonUtils.h>

bool JsonUtils::HasKey(const Json& json, const std::string& key)
{
	return json.find(key) != json.end();
}
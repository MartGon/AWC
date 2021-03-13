#include <DB/JsonUtils.h>

#include <fstream>

bool JsonUtils::HasKey(const Json& json, const std::string& key)
{
	bool hasKey = json.find(key) != json.end();
	return hasKey;
}

Json JsonUtils::GetJsonFromFile(std::string filename, std::string folder)
{
    const std::string path = folder + filename;

    std::ifstream file{path};

    Json data;
	if(file.good())
		data = Json::parse(file);

    file.close();

    return data;
}
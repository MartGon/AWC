#include <json.hpp>

using Json = nlohmann::json;

namespace AWCSerTest
{
    Json GetJsonFromFile(std::string filePath);
}
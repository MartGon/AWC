#include <Utils/STLUtils.h>

#include <sstream>

std::string StringUtils::Replace(std::string string, const std::string& token, const std::string& replacer)
{
    auto tokenPos = string.find(token);
    if(tokenPos < string.size())
        string.replace(tokenPos, token.size(), replacer);

    return string;
}

std::vector<std::string> StringUtils::Split(const std::string& string, char delim)
{
    std::vector<std::string> items;

    std::istringstream iss(string);
    std::string item;
    while(std::getline(iss, item, delim))
        items.push_back(item);

    return items;
}
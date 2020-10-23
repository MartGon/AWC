#include <Utils/STLUtils.h>

std::string StringUtils::Replace(std::string string, const std::string& token, const std::string& replacer)
{
    auto tokenPos = string.find(token);
    if(tokenPos < string.size())
        string.replace(tokenPos, token.size(), replacer);

    return string;
}
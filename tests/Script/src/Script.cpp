#include <Test/Script/Script.h>


std::string Test::Script::GetUserDataPath(std::string str)
{
    return std::string(SCRIPTS_DIR) + "UserData/" + str;
}

std::string Test::Script::GetGameScriptPath(std::string filename)
{
    return std::string(SCRIPTS_DIR) + "Game/" + filename;
}
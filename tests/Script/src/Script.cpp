#include <Test/Script/Script.h>


std::string Test::Script::GetUserDataPath()
{
    return std::string(SCRIPTS_DIR) + "UserData/";
}

std::string Test::Script::GetGameScriptPath(std::string filename)
{
    return std::string(SCRIPTS_DIR) + "Game/" + filename;
}
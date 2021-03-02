#include <Test/Script/UserData/UserData.h>

using namespace Test::Script;

std::string UserData::GetUserDataPath()
{
    return std::string(SCRIPTS_DIR) + "UserData/";
}
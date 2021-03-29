#include <Script/UserData.h>

using namespace Script;

const char* UserData::Process::MT_NAME = "AWC_Process";
const char* UserData::Process::LIB_NAME = "Process";

const luaL_Reg UserData::Process::methods[] = {
    {"__index", Process::Get},
    {NULL, NULL}
};

const luaL_Reg UserData::Process::functions[] = {
    {NULL, NULL}
};

int UserData::Process::Get(lua_State* L)
{
    auto p = UserData::CheckUserData<Process>(L, 1);
    std::string index = std::string(luaL_checkstring(L, 2));

    int ret = 1;
    if(index == "priority")
    {
        lua_pushinteger(L, p->priority);
    }
    else if(index == "trigger")
    {
        UserData::PushDataCopy<Trigger>(L, p->trigger);
    }
    else if(index == "id")
    {
        lua_pushinteger(L, p->id);
    }
    else
        ret = 0;

    return ret;
}
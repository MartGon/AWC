#include <Script/UserData.h>

#include <Script/Wrappers/LuaTable.h>

using namespace Script;

const char* UserData::Trigger::MT_NAME = "AWC_Trigger";
const char* UserData::Trigger::LIB_NAME = "Trigger";

const luaL_Reg UserData::Trigger::methods[] = {
    {"__index", Trigger::Get},
    {NULL, NULL}
};

const luaL_Reg UserData::Trigger::functions[] = {
    {NULL, NULL}
};

unsigned int ToInt(Process::Trigger::Type type)
{
    return static_cast<unsigned int>(type);
}

int UserData::Trigger::InitLib(lua_State* L)
{
    lua_getglobal(L, LIB_NAME);
    Script::LuaTable<Scope::External> libTable{L, -1};
    lua_pop(L, 1);

    libTable.Set("PLAYER", ToInt(::Process::Trigger::Type::PLAYER));
    libTable.Set("OPERATION", ToInt(::Process::Trigger::Type::OPERATION));

    return 0;
}

int UserData::Trigger::Get(lua_State* L)
{
    auto t = UserData::CheckUserData<Trigger>(L, 1);
    std::string index = std::string(luaL_checkstring(L, 2));

    int ret = 1;
    if(index == "id")
    {
        lua_pushinteger(L, t->id);
    }
    else if(index == "type")
    {
        lua_pushinteger(L, ToInt(t->type));
    }
    else
        ret = 0;

    return ret;
}
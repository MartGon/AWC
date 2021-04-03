#include <Script/UserData.h>

#include <Script/Wrappers/LuaTable.h>

using namespace Script;

const char* UserData::EventNotification::MT_NAME = "AWC_EventNotification";
const char* UserData::EventNotification::LIB_NAME = "EventNotification";
const luaL_Reg UserData::EventNotification::methods[] = {
    {NULL, NULL}
};
const luaL_Reg UserData::EventNotification::functions[] = {
    {NULL, NULL}
};

int UserData::EventNotification::InitLib(lua_State* L)
{
    lua_getglobal(L, LIB_NAME);
    Script::LuaTable<Scope::External> libTable{L, -1};
    lua_pop(L, 1);
    Script::LuaTable<Scope::External> typeTable{L};
    libTable.SetLuaWrapper("Type", typeTable);

    typeTable.Set("NONE", ToInt(::Event::Notification::Type::NONE));
    typeTable.Set("PRE", ToInt(::Event::Notification::Type::PRE));
    typeTable.Set("POST", ToInt(::Event::Notification::Type::POST));
    typeTable.Set("ERROR", ToInt(::Event::Notification::Type::ERROR));
    typeTable.Set("ANY", ToInt(::Event::Notification::Type::ANY));

    return 0;
}

void UserData::EventNotification::ToTable(lua_State* L, type& notification)
{
    LuaTable<Scope::Internal> notiTable{L};
    notiTable.Set("type", ToInt(notification.type));
    notiTable.SetAsTable<Process>("process", notification.process);

    // Pushed before destructor call, that way we avoid Lua from garbage collecting it
    notiTable.PushInternal();
}
#include <Script/UserData.h>

#include <Script/Wrappers/LuaFunction.h>
#include <Script/Wrappers/LuaCallback.h>

using namespace Script;

const char* UserData::EventHandler::MT_NAME = "AWC_EventHandler";
const char* UserData::EventHandler::LIB_NAME = "EventHandler";
const luaL_Reg UserData::EventHandler::methods[] = {
    {"New", EventHandler::New},
    {NULL, NULL}
};
const luaL_Reg UserData::EventHandler::functions[] = {
    {NULL, NULL}
};

int UserData::EventHandler::New(lua_State* luaState)
{
    FromTable(luaState, 1);

    return 1;
}

UserData::EventHandler::type* UserData::EventHandler::FromTable(lua_State* luaState, int index)
{
    LuaTable<Scope::Internal> table{luaState, index};
    unsigned int opType = table.Get<unsigned int>("opType");
    auto func = table.GetLuaWrapper<LuaFunction<Scope::Internal>>("callback");
    func->PushInternal();
    std::shared_ptr<Event::CallbackI> cb = std::make_shared<LuaCallback>(luaState, -1);
    lua_pop(luaState, 1);

    auto notiType = static_cast<Event::Notification::Type>(table.Get<unsigned int>("notiType"));
    bool isValid = notiType >= Event::Notification::Type::NONE && notiType <= Event::Notification::Type::ANY; 
    CheckArg<Scope::Internal>(luaState, isValid, index, "Notification type is not valid");

    return UserData::PushDataCopy<EventHandler>(luaState, Event::Handler{opType, cb, notiType});
}
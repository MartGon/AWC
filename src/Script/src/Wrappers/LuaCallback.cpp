#include <Script/Wrappers/LuaCallback.h>

#include <Script/Wrappers/LuaTable.h>
#include <Script/UserData.h>

#ifdef _DEBUG
    #include <iostream>
#endif

void Script::LuaCallback::Call(const Event::Notification::Notification noti, Entity::GUID e, ::Game& game)
{
    f.PushInternal();

    UserData::UserData::PushDataCopy<UserData::EventNotification>(L, noti);
    UserData::UserData::PushDataCopy<UserData::EntityGUID>(L, e);
    UserData::UserData::PushDataRef<UserData::Game>(L, &game);
    
    auto result = lua_pcall(L, 3, 0, 0);
    if(result != LUA_OK)
    {
        std::string error = lua_tostring(L, -1);
        lua_pop(L, 1);
        #ifdef _DEBUG
            std::cout << "Lua Error while handling an event: " << error << '\n';
        #endif
    }
}
#include <Script/Wrappers/LuaCallback.h>

#include <Script/Wrappers/LuaTable.h>
#include <Script/UserData.h>

#ifdef _DEBUG
    #include <iostream>
#endif

void Script::LuaCallback::Call(const Event::Notification::Notification noti, Entity::GUID e, ::Game& game)
{
    f.PushInternal();

    auto notification = noti;
    UserData::UserData::PushAsTable<UserData::EventNotification>(L, notification);
    UserData::UserData::PushDataCopy<UserData::EntityGUID>(L, e);
    
    auto result = lua_pcall(L, 2, 0, 0);
    if(result != LUA_OK)
    {
        std::string error = lua_tostring(L, -1);
        lua_pop(L, 1);
        #ifdef _DEBUG
            std::cout << "Lua Error while handling an event: " << error << '\n';
        #endif
    }
}
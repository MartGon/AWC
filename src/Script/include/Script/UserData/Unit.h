#pragma

#include <AWC.h>

#include <lua.hpp>

namespace Script::UserData::Unit
{
    extern const char* MT_NAME;
    extern const luaL_Reg methods[];
    
    void Init(lua_State* luaState);
    void Push(lua_State* luaState, ::Unit* unit);

    int CalculateMovement(lua_State* luaState);
}
#include <Script/Wrappers/LuaFunction.h>

using namespace Script;

LuaFunction::LuaFunction(lua_State* luaState, int index) : luaState_{luaState}
{
    CheckType(index, lua_gettop(luaState));
    lua_pushvalue(luaState, index);
    functionRef_ = luaL_ref(luaState, LUA_REGISTRYINDEX);
}

LuaFunction::LuaFunction(const LuaFunction& other) : luaState_{other.luaState_}, 
    functionRef_{(lua_rawgeti(other.luaState_, LUA_REGISTRYINDEX, other.functionRef_), luaL_ref(other.luaState_, LUA_REGISTRYINDEX))}
{

}

LuaFunction::LuaFunction(LuaFunction&& other) : luaState_{other.luaState_}, functionRef_{other.functionRef_}
{
    other.functionRef_ = LUA_REFNIL;
}

LuaFunction::~LuaFunction()
{
    luaL_unref(luaState_, LUA_REGISTRYINDEX, functionRef_);
}

void LuaFunction::PushFunction() const
{
    lua_rawgeti(luaState_, LUA_REGISTRYINDEX, functionRef_);
}

// private

void LuaFunction::CheckType(int index, int top)
{
    auto type = lua_type(luaState_, index);
    if(type != LUA_TFUNCTION)
    {
        lua_settop(luaState_, top);
        throw AWCException("LuaFunction: value at index " + std::to_string(index) + " was not a function");
    }
}
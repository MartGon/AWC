#include <Script/Wrappers/LuaFunction.h>

using namespace Script;

// Construction

LuaFunction::LuaFunction(lua_State* luaState) : luaState_{luaState}, functionRef_{LUA_REFNIL}
{

}

LuaFunction::LuaFunction(lua_State* luaState, int index) : luaState_{luaState}
{
    CheckType(index, lua_gettop(luaState));
    lua_pushvalue(luaState, index);
    functionRef_ = luaL_ref(luaState, LUA_REGISTRYINDEX);
}

// Copy
LuaFunction::LuaFunction(const LuaFunction& other) : luaState_{other.luaState_}, 
    functionRef_{(other.PushInternal(), luaL_ref(other.luaState_, LUA_REGISTRYINDEX))}
{

}

LuaFunction& LuaFunction::operator=(const LuaFunction& other)
{
    LuaFunction temp{other};
    swap(*this, temp);

    return *this;
}

// Move
LuaFunction::LuaFunction(LuaFunction&& other) : luaState_{other.luaState_}, functionRef_{other.functionRef_}
{
    other.functionRef_ = LUA_REFNIL;
}

LuaFunction& LuaFunction::operator=(LuaFunction&& other)
{
    luaL_unref(luaState_, LUA_REGISTRYINDEX, functionRef_);

    functionRef_ = other.functionRef_;
    luaState_ = other.luaState_;

    other.functionRef_ = LUA_REFNIL;

    return *this;
}

LuaFunction::~LuaFunction()
{
    luaL_unref(luaState_, LUA_REGISTRYINDEX, functionRef_);
}

// public

int LuaFunction::PushInternal() const
{
    return lua_rawgeti(luaState_, LUA_REGISTRYINDEX, functionRef_);
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
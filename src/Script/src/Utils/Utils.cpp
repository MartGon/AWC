#include <Script/Utils/Utils.h>

#include <Script/Wrappers/LuaTable.h>
#include <Script/Wrappers/LuaFunction.h>

using namespace Script;

// Push and To

template<>
void Script::Push<int>(lua_State* state, int val)
{
    lua_pushinteger(state, val);
}

template<>
void Script::Push<std::string>(lua_State* state, std::string val)
{
    lua_pushstring(state, val.c_str());
}

template<>
void Script::Push<const char*>(lua_State* state, const char* val)
{
    lua_pushstring(state, val);
}

template<>
void Script::Push<bool>(lua_State* state, bool val)
{
    lua_pushboolean(state, val);
}

template<>
void Script::Push<Script::LuaTable&>(lua_State* state, Script::LuaTable& val)
{
    val.PushInternal();
}

template<>
void Script::Push<Script::LuaFunction&>(lua_State* state, Script::LuaFunction& val)
{
    val.PushInternal();
}

template<>
int Script::To<int>(lua_State* state, int index)
{
    return lua_tointeger(state, index);
}

template<>
unsigned int Script::To<unsigned int>(lua_State* state, int index)
{
    return (unsigned int)lua_tointeger(state, index);
}

template<>
std::string Script::To<std::string>(lua_State* state, int index)
{
    return std::string(lua_tostring(state, index));
}

template<>
Script::LuaTable Script::To<Script::LuaTable>(lua_State* state, int index)
{
    return LuaTable{state, index};
}

template<>
Script::LuaFunction Script::To<Script::LuaFunction>(lua_State* state, int index)
{
    return LuaFunction{state, index};
}

template<>
bool Script::To<bool>(lua_State* state, int index)
{
    return lua_toboolean(state, index);
}

template<>
int Script::GetField<int>(lua_State* luaState, int index, int key)
{
    return lua_geti(luaState, index, key);
}

template<>
int Script::GetField<const char*>(lua_State* luaState, int index, const char* key)
{
    return lua_getfield(luaState, index, key);
}

template<>
int Script::GetField<std::string>(lua_State* luaState, int index, std::string key)
{
    return lua_getfield(luaState, index, key.c_str());
}

bool Script::IsTable(lua_State* luaState, int index)
{
    return lua_type(luaState, index) == LUA_TTABLE;
}
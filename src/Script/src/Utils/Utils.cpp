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

bool Script::IsFunction(lua_State* luaState, int index)
{
    return lua_type(luaState, index) == LUA_TFUNCTION;
}

template<>
void Script::Check<Script::Scope::Internal>(lua_State* luaState, bool condition, std::string msg)
{
    if(!condition)
    {
        luaL_error(luaState, msg.c_str());
    }
}

template<>
void Script::Check<Script::Scope::External>(lua_State* luaState, bool condition, std::string msg)
{
    if(!condition)
    {
        throw AWCException(msg);
    }
}

template<>
void Script::CheckArg<Scope::Internal>(lua_State* luaState, bool condition, int index, std::string msg)
{
    luaL_argcheck(luaState, condition, index, msg.c_str());
}

template<>
void Script::CheckArg<Scope::External>(lua_State* luaState, bool condition, int index, std::string msg)
{
    if(!condition)
        throw AWCException("Error at argument #" + std::to_string(index) + ": " + msg);
}

template<>
void Script::CheckExpectedArg<Scope::Internal>(lua_State* luaState, bool condition, int index, std::string type)
{
    luaL_argexpected(luaState, condition, index, type.c_str());
}

template<>
void Script::CheckExpectedArg<Scope::External>(lua_State* luaState, bool condition, int index, std::string type)
{
    if(!condition)
        throw AWCException("Bad argument #" + std::to_string(index) + ": Expected " + type + 
            " got " + std::string(lua_typename(luaState, index)));
}
#include <Script/LuaTable.h>

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
    return lua_geti(luaState, -1, key);
}

template<>
int Script::GetField<const char*>(lua_State* luaState, int index, const char* key)
{
    return lua_getfield(luaState, -1, key);
}

template<>
int Script::GetField<std::string>(lua_State* luaState, int index, std::string key)
{
    return lua_getfield(luaState, -1, key.c_str());
}

// LuaTable

LuaTable::LuaTable(lua_State* luaState, int index) : luaState_{luaState}
{
    lua_pushvalue(luaState, index);
    tableRef_ = luaL_ref(luaState, LUA_REGISTRYINDEX);
}

LuaTable::LuaTable(lua_State* luaState) : luaState_{luaState}
{
    lua_newtable(luaState);
    tableRef_ = luaL_ref(luaState, LUA_REGISTRYINDEX);
}

LuaTable::LuaTable(lua_State* luaState, std::string mtName) : luaState_{luaState}, mtName_{mtName}
{
    lua_newtable(luaState);
    tableRef_ = luaL_ref(luaState, LUA_REGISTRYINDEX);
    SetMetaTable(mtName);
}

LuaTable::LuaTable(LuaTable&& lt)
{
    this->tableRef_ = lt.tableRef_;
    this->luaState_ = lt.luaState_;
    this->mtName_ = lt.mtName_;
}

LuaTable::~LuaTable()
{
    luaL_unref(luaState_, LUA_REGISTRYINDEX, tableRef_);   
}

// Public

int LuaTable::Length()
{
    PushLuaTable();
    int len = lua_rawlen(luaState_, -1);
    lua_pop(luaState_, 1);

    return len;
}

void LuaTable::SetMetaTable(std::string mtName)
{
    PushLuaTable();
    luaL_setmetatable(luaState_, mtName.c_str());
    lua_pop(luaState_, 1);
}

std::string LuaTable::GetMetaTableName()
{
    std::string name;

    PushLuaTable();
    if(lua_getmetatable(luaState_, -1))
    {
        lua_getfield(luaState_, -1, "__name");
        name = std::string(lua_tostring(luaState_, -1));
        lua_pop(luaState_, 2); // Pops metatable and __name value
    }
    lua_pop(luaState_, 1);

    return name;
}

void LuaTable::PushLuaTable()
{
    int type = lua_rawgeti(luaState_, LUA_REGISTRYINDEX, tableRef_);
    if(type != LUA_TTABLE)
        throw AWCException("LuaTable: Internal Lua table was not type LUA_TTABLE");
}

// Private

// Assumes that table is at -2 and value is at the top of the stack
// Pops the internal lua table from the stack
void LuaTable::SetField(std::string key)
{
    lua_setfield(luaState_, -2, key.c_str());
}

void LuaTable::SetField(int index)
{
    lua_seti(luaState_, -2, index);
}
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

std::unique_ptr<LuaTable> LuaTable::GetTable(std::string key)
{
    PushLuaTable();
    int type = lua_getfield(luaState_, -1, key.c_str());
    
    return std::unique_ptr<LuaTable>(new LuaTable{luaState_, -1});
}

std::unique_ptr<LuaTable> LuaTable::GetTable(int index)
{
    PushLuaTable();
    lua_geti(luaState_, -1, index);

    return std::unique_ptr<LuaTable>(new LuaTable{luaState_, -1});
}

void LuaTable::SetMetaTable(std::string mtName)
{
    PushLuaTable();
    luaL_setmetatable(luaState_, mtName.c_str());
    lua_pop(luaState_, 1);
}

int LuaTable::GetKeyType(std::string key)
{
    PushLuaTable();
    int type = lua_getfield(luaState_, -1, key.c_str());
    lua_pop(luaState_, 2);

    return type;
}

int LuaTable::GetIndexType(int index)
{
    PushLuaTable();
    int type = lua_geti(luaState_, -1, index);
    lua_pop(luaState_, 2);
    
    return type;
}

bool LuaTable::ContainsValue(std::string key)
{            
    return GetKeyType(key) != LUA_TNIL;
}

int LuaTable::Length()
{
    PushLuaTable();
    int len = lua_rawlen(luaState_, -1);
    lua_pop(luaState_, 1);

    return len;
}

// Private

void LuaTable::PushLuaTable()
{
    int type = lua_rawgeti(luaState_, LUA_REGISTRYINDEX, tableRef_);
    if(type != LUA_TTABLE)
        throw AWCException("LuaTable: Internal Lua table was not type LUA_TTABLE");
}

// Assumes that table is at -2 and value is at the top of the stack
// Pops the internal lua table from the stack
void LuaTable::SetField(std::string key)
{
    lua_setfield(luaState_, -2, key.c_str());
    lua_pop(luaState_, 1);
}

void LuaTable::SetField(int index)
{
    lua_seti(luaState_, -2, index);
    lua_pop(luaState_, 1);
}
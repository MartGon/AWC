#include <Script/Wrappers/LuaTable.h>

using namespace Script;

// LuaTable

LuaTable::LuaTable(lua_State* luaState, int index) : luaState_{luaState}
{
    CheckType(index, lua_gettop(luaState));
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

LuaTable::LuaTable(LuaTable&& other) : tableRef_{other.tableRef_}, luaState_{other.luaState_}, mtName_{other.mtName_}
{
    other.tableRef_ = LUA_REFNIL;
}

LuaTable& LuaTable::operator=(LuaTable&& other)
{
    luaL_unref(luaState_, LUA_REGISTRYINDEX, tableRef_);

    tableRef_ = other.tableRef_;
    luaState_ = other.luaState_;
    mtName_ = std::move(other.mtName_);

    other.tableRef_ = LUA_REFNIL;

    return *this;
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
    lua_rawgeti(luaState_, LUA_REGISTRYINDEX, tableRef_);
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

void LuaTable::CheckType(int index, int top)
{
    auto type = lua_type(luaState_, index);
    if(type != LUA_TTABLE)
    {
        lua_settop(luaState_, top);
        throw AWCException("LuaTable: Internal Lua table was not type LUA_TTABLE");
    }
}
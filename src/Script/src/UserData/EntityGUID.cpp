#include <Script/UserData.h>

#include <Script/Wrappers/LuaTable.h>

using namespace Script;

const char* UserData::EntityGUID::MT_NAME = "AWC_EntityGUID";
const char* UserData::EntityGUID::LIB_NAME = "EntityGUID";
const luaL_Reg UserData::EntityGUID::methods[] = {
    {"__eq", EntityGUID::Eq},
    {"__index", EntityGUID::Index},
    {"__tostring", EntityGUID::ToString},
    {NULL, NULL}
};
const luaL_Reg UserData::EntityGUID::functions[] = {
    {NULL, NULL}
};

UserData::EntityGUID::type* UserData::EntityGUID::FromTable(lua_State* L, int index)
{
    LuaTable<Scope::Internal> table{L, index};

    auto type = table.Get<::Entity::Type>("type");
    auto subType = table.Get<unsigned int>("subtype");
    auto id = table.Get<unsigned int>("id");

    return UserData::PushDataCopy<EntityGUID>(L, ::Entity::GUID{type, subType, id});
}

void UserData::EntityGUID::ToTable(lua_State* L, type& guid)
{
    LuaTable<Scope::Internal> table{L};
    table.Set("type", ToInt(guid.type));
    table.Set("subType", guid.subType);
    table.Set("id", guid.id);

    table.PushInternal();
}

int UserData::EntityGUID::Eq(lua_State* L)
{
    auto a = *UserData::CheckUserData<EntityGUID>(L, 1);
    auto b = *UserData::CheckUserData<EntityGUID>(L, 2);

    lua_pushboolean(L, a == b);

    return 1;
}

int UserData::EntityGUID::Index(lua_State* L)
{
    auto GUID = UserData::CheckUserData<EntityGUID>(L, 1);
    auto key  = std::string(lua_tostring(L, 2));

    if(key == "type")
        Push(L, GUID->type);
    else if(key == "subType")
        lua_pushinteger(L, GUID->subType);
    else if(key == "id")
        lua_pushinteger(L, GUID->id);

    return 1;
}

int UserData::EntityGUID::ToString(lua_State* L)
{
    auto GUID = UserData::CheckUserData<EntityGUID>(L, 1);
    lua_pushstring(L, GUID->ToString().c_str());

    return 1;
}
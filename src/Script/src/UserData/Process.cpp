#include <Script/UserData.h>

#include <Script/Wrappers/LuaTable.h>

using namespace Script;

const char* UserData::Process::MT_NAME = "AWC_Process";
const char* UserData::Process::LIB_NAME = "Process";

const luaL_Reg UserData::Process::methods[] = {
    {"__index", Process::Get},
    {NULL, NULL}
};

const luaL_Reg UserData::Process::functions[] = {
    {NULL, NULL}
};

int UserData::Process::Get(lua_State* L)
{
    auto p = UserData::CheckUserData<Process>(L, 1);
    std::string index = std::string(luaL_checkstring(L, 2));

    int ret = 1;
    if(index == "priority")
    {
        lua_pushinteger(L, p->priority);
    }
    else if(index == "trigger")
    {
        UserData::PushDataCopy<Trigger>(L, p->trigger);
    }
    else if(index == "id")
    {
        lua_pushinteger(L, p->id);
    }
    else if(index == "operation")
    {
        UserData::PushDataCopy<Operation>(L, p->op);
    }
    else
        ret = 0;

    return ret;
}

UserData::Process::type* UserData::Process::FromTable(lua_State* L, int index)
{
    LuaTable<Scope::Internal> table{L, index};

    unsigned int id = table.Get<unsigned int>("id");
    auto announced = table.Get<bool>("announced");
    auto prio = table.Get<uint8_t>("priority");
    auto operation = *table.GetUserData<Operation>("operation");
    auto trigger = *table.GetUserData<Trigger>("trigger");

    ::Process::Process p{id, operation, trigger, prio};
    p.announced = false;
    return UserData::PushDataCopy<Process>(L, p);
}

void UserData::Process::ToTable(lua_State* L, type& p)
{
    LuaTable<Scope::Internal> table{L};

    table.Set("id", p.id);
    table.Set("announced", p.announced);
    table.Set("priority", p.priority);
    table.SetDataCopy<Operation>("operation", p.op);
    table.SetAsTable<Trigger>("trigger", p.trigger);

    table.PushInternal();
}
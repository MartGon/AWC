#include <Script/UserData.h>

#include <Script/Wrappers/LuaTable.h>
#include <Script/ScriptOperation.h>

using namespace Script;

const char* UserData::Operation::MT_NAME = "AWC_Operation";
const char* UserData::Operation::LIB_NAME = "Operation";

const luaL_Reg UserData::Operation::methods[] = {
    {"GetArgs", GetArgs},
    {NULL, NULL}
};

const luaL_Reg UserData::Operation::functions[] = {
    {NULL, NULL}
};

void UserData::Operation::ToTable(lua_State* L, type& op)
{
    LuaTable<Scope::Internal> table{L};

    table.Set("type", op->GetType());

    table.PushInternal();
}

int UserData::Operation::GetArgs(lua_State* L)
{
    auto op = *UserData::CheckUserData<Operation>(L, 1);
    CheckArg<Scope::Internal>(L, op->GetType() == Script::SCRIPT, 1, "Operation is not of type Script");

    auto sop = op->To<ScriptOperation>();
    auto& args = sop->GetArgsTable();
    args.PushInternal();

    return 1;
}
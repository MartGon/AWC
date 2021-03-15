#include <Script/UserData/AreaDesc.h>
#include <Script/UserData/Vector2.h>

#include <Script/LuaTable.h>

using namespace Script;

const char* UserData::AreaDesc::MT_NAME = "AWC_AreaDesc";
const char* UserData::AreaDesc::LIB_NAME = "AreaDesc";

const luaL_Reg UserData::AreaDesc::methods[] = {
    {NULL, NULL}
};

const luaL_Reg UserData::AreaDesc::functions[] = {
    {"New", AreaDesc::New},
    {NULL, NULL}
};

int UserData::AreaDesc::New(lua_State* luaState)
{
    bool isTable = lua_istable(luaState, 1);
    luaL_argcheck(luaState, isTable, 1, "Expected table");

    LuaTable lt{luaState, 1};

    const std::string dirKey = "directions";
    auto dirTable = lt.GetTable(dirKey);

    const std::string error = dirKey + " key did not exist on table";
    luaL_argcheck(luaState, dirTable, 1, error.c_str());

    std::vector<::Vector2> dirs;
    auto len = dirTable->Length();
    for(int i = 1; i < len + 1; i++)
    {
        auto type = dirTable->GetType(i);
        auto vec = dirTable->GetUserData<Vector2>(i);
        dirs.push_back(*vec);
    }

    ::AreaDescPtr tpdp = ::AreaDesc::Create(dirs);
    UserData::PushDataCopy<AreaDesc>(luaState, tpdp);
    
    return 1;
}

/*
    --- Option 1 ----
    AreaDesc.New({
        "directions" = 
        {
            Vector2.New(1, 0),
            Vector2.New(0, 1),
            Vector2.New(-1, 0)
            Vector2.New(0, -1)
        }
    })

    --- Option 2 ---
    AreaDesc.New({
        "directions" = 
        {
            {x = 1, y = 0},
            {x = 0, y = 1},
            {x = -1, y = 0},
            {x = 0, y = -1},
        }
    })
*/

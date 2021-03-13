#include <Script/UserData/TilePatternDesc.h>
#include <Script/UserData/Vector2.h>

#include <Script/LuaTable.h>

using namespace Script;

const char* UserData::TilePatternDesc::MT_NAME = "AWC_TilePatternDesc";
const char* UserData::TilePatternDesc::LIB_NAME = "TilePatternDesc";

const luaL_Reg UserData::TilePatternDesc::methods[] = {
    {NULL, NULL}
};

const luaL_Reg UserData::TilePatternDesc::functions[] = {
    {"New", TilePatternDesc::New},
    {NULL, NULL}
};

int UserData::TilePatternDesc::New(lua_State* luaState)
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
        auto type = dirTable->GetIndexType(i);
        auto vec = dirTable->GetUserData<Vector2>(i);
        dirs.push_back(*vec);
    }

    ::TilePatternDescPtr tpdp = ::TilePatternDesc::Create(dirs);
    UserData::PushGCData<TilePatternDesc>(luaState, tpdp);
    
    return 1;
}

/*
    --- Option 1 ----
    TilePatternDesc.New({
        "directions" = 
        {
            Vector2.New(1, 0),
            Vector2.New(0, 1),
            Vector2.New(-1, 0)
            Vector2.New(0, -1)
        }
    })

    --- Option 2 ---
    TilePatternDesc.New({
        "directions" = 
        {
            {x = 1, y = 0},
            {x = 0, y = 1},
            {x = -1, y = 0},
            {x = 0, y = -1},
        }
    })
*/

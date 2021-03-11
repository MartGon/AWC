#include <Script/UserData/TilePatternDesc.h>
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
    auto dirT = lt.GetTable(dirKey);

    const std::string error = dirKey + " key did not exist on table";
    luaL_argcheck(luaState, dirT.has_value(), 1, error.c_str());

    


    
    return 1;
}

/*
    TilePatternDesc.New({
        "directions" = 
        {
            Vector2.New(0, 0),

        }
    })
*/

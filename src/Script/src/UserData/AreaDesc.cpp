#include <Script/UserData/AreaDesc.h>
#include <Script/UserData/Vector2.h>

#include <Script/Wrappers/LuaTable.h>

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

std::string CreateKeyError(const std::string& key)
{
    return key + " key did not exist on table";
}

std::vector<::Vector2> ParseDirs(LuaTable& dirTable)
{
    std::vector<::Vector2> dirs;
    for(int i = 1; i < dirTable.Length() + 1; i++)
    {
        auto vec = dirTable.GetUserData<Script::UserData::Vector2>(i);
        dirs.push_back(*vec);
    }

    return dirs;
}

int UserData::AreaDesc::New(lua_State* luaState)
{
    // Value to push
    ::AreaDescPtr tpdp;

    bool isTable = lua_istable(luaState, 1);
    luaL_argcheck(luaState, isTable, 1, "Expected table");

    LuaTable lt{luaState, 1};

    const std::string dirKey = "directions";
    auto dirTable = lt.GetLuaWrapper<Script::LuaTable>(dirKey);

    const std::string error = CreateKeyError(dirKey);
    luaL_argcheck(luaState, dirTable, 1, error.c_str());

    // Parse Dirs
    auto dirs = ParseDirs(dirTable.value());

    const std::string lockedDirKey = "lockedDirs";
    const std::string exclusiveDirKey = "exclusiveDirs";
    
    // Parse locked dirs
    if(lt.ContainsValue(lockedDirKey))
    {
        auto luaLockedDirsTable = lt.GetLuaWrapper<LuaTable>(lockedDirKey);
        DirectionsTable lockedDirsTable;

        for(int i = 1; i < luaLockedDirsTable->Length() + 1; i++)
        {
            auto type = luaLockedDirsTable->GetType(i);
            luaL_argcheck(luaState, type == LUA_TTABLE, 1, "Entry of the given DirectionsTable was not a table");
            
            auto entry = luaLockedDirsTable->GetLuaWrapper<LuaTable>(i);
            auto dir = entry->GetUserData<Vector2>("dir");
            type = entry->GetType("locksTo");
            luaL_argcheck(luaState, type == LUA_TTABLE, 1, "locksTo entry was not a table");

            auto lockedDirs = entry->GetLuaWrapper<LuaTable>("locksTo");
            auto dirs = ParseDirs(lockedDirs.value());

            lockedDirsTable.insert({*dir, dirs});
        }

        tpdp = ::AreaDesc::CreateByLocked(dirs, lockedDirsTable);
    }
    else
        tpdp = ::AreaDesc::Create(dirs);

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
        },
        "lockedDirs" = {
            { 
                dir = Vector2.New(1, 0), 
                locksTo = {
                    Vector2.New(1, 0)
                }            
            },
            {
                dir = Vector2.New(0, 1), 
                locksTo = {
                    Vector2.New(0, 1)
                }            
            }
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

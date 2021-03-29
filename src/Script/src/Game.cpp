#include <Script/Game.h>

#include <Script/UserData/Database.h>

#include <AWC/Exception.h>


unsigned int Script::Game::CreateScriptType(std::string scriptPath)
{
    auto& scriptTypesTable = db.get<Script::Type>();
    auto id = scriptTypesTable.Emplace(vm_, scriptPath);

    return id;
}

unsigned int Script::Game::CreateScript(unsigned int typeId)
{
    unsigned int id = -1;
    auto& types = db.get<Script::Type>();
    auto sType = types.GetById(typeId);
    
    if(sType)
    {
        auto script = sType->CreateScript();
        
        auto& scripts = db.get<std::shared_ptr<Script::ScriptOperation>>();
        id = scripts.Add(script);
    }

    return id;
}

Script::LuaTable<Script::Scope::External>& Script::Game::GetScriptTable(unsigned int id)
{
    auto& scripts = db.get<std::shared_ptr<ScriptOperation>>();
    auto scriptPtr  = scripts.GetById(id);
    if(scriptPtr)
    {
        auto script = *scriptPtr;
        return script->GetArgsTable();
    }
    else
        throw AWC::Exception("GetScriptTable Error: Script with id " + std::to_string(id) + " did not exist");
}

void Script::Game::RunConfig(std::string configPath)
{
    vm_.RunFile(configPath);
}

// Private

void Script::Game::InitState()
{
    // Init userdata
    auto L = vm_.GetLuaState();
    UserData::Init(L);

    // Open standard libs
    luaL_openlibs(L);

    // Push database
    UserData::UserData::PushDataRef<UserData::Database>(L, &db);
    lua_setglobal(L, "DB");

    // Push game
    UserData::UserData::PushDataRef<UserData::Game>(L, &game_);
    lua_setglobal(L, "game");

    // Create _MAIN, _ENV metatable, to access _G when not found
    luaL_newmetatable(L, MT_NAME);

    int type = lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
    lua_setfield(L, -2, "__index"); // _MAIN.__index = _G
    lua_pop(L, 1);
}
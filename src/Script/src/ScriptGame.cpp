#include <Script/ScriptGame.h>

#include <AWC/AWCException.h>


unsigned int Script::Game::CreateScriptType(std::string scriptPath)
{
    Script::Type st{ls.GetLuaState(), scriptPath};
    auto id = stIndex_++;
    scriptTypes_.insert({id, st});

    return id;
}

unsigned int Script::Game::CreateScript(unsigned int typeId)
{
    unsigned int id = -1;
    if(UnorderedMapUtils::Contains(scriptTypes_, typeId))
    {
        auto& st = scriptTypes_.at(typeId);
        auto script = st.CreateScript();

        id = sIndex_++;
        scripts_.insert({id, script});
    }

    return id;
}

Script::LuaTable& Script::Game::GetScriptTable(unsigned int id)
{
    if(UnorderedMapUtils::Contains(scripts_, id))
    {
        auto& script = scripts_.at(id);
        return script->GetArgsTable();
    }
    else
        throw AWCException("GetScriptTable Error: Script with id " + std::to_string(id) + " did not exist");
}

unsigned int Script::Game::PushScript(unsigned int id, unsigned int prio)
{
    unsigned int pid = -1;
    if(UnorderedMapUtils::Contains(scripts_, id))
    {
        auto& script = scripts_.at(id);
        pid = game_.Push(script, prio);

        //scripts_.erase(id);
    }

    return pid;
}

// Private

void Script::Game::InitState()
{
    // Init userdata
    auto L = ls.GetLuaState();
    UserData::Init(L);

    // Open standard libs
    luaL_openlibs(L);

    // Create _MAIN, _ENV metatable, to access _G when not found
    luaL_newmetatable(L, MT_NAME);

    int type = lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
    lua_setfield(L, -2, "__index"); // _MAIN.__index = _G
    lua_pop(L, 1);
}
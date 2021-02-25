#include <AWC/ScriptGame.h>

#include <AWC/AWCException.h>

unsigned int ScriptGame::CreateScriptType(std::string scriptPath)
{
    Operation::ScriptType st{ls.GetLuaState(), scriptPath};
    auto id = stIndex_++;
    scriptTypes_.insert({id, st});

    return id;
}

unsigned int ScriptGame::CreateScript(unsigned int typeId)
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

LuaTable& ScriptGame::GetScriptTable(unsigned int id)
{
    if(UnorderedMapUtils::Contains(scripts_, id))
    {
        auto& script = scripts_.at(id);
        return script->GetArgsTable();
    }
    else
        throw AWCException("GetScriptTable Error: Script with id " + std::to_string(id) + " did not exist");
}

void ScriptGame::PushScript(unsigned int id, unsigned int prio)
{
    if(UnorderedMapUtils::Contains(scripts_, id))
    {
        auto& script = scripts_.at(id);
        game_.Push(script, prio);

        //scripts_.erase(id);
    }
}
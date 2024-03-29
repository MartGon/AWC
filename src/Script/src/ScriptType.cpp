#include <Script/ScriptType.h>
#include <Script/Game.h>
#include <Script/UserData/Game.h>

#include <Script/Wrappers/LuaVM.h>
#include <Script/Wrappers/LuaTable.h>

#include <AWC/Exception.h>

#ifdef _DEBUG
    #include <iostream>
#endif

using namespace Script;

Script::Type::Type(LuaVM& vm, std::string scriptPath) : scriptPath_{scriptPath}, vm_{vm}, execute_{vm.GetLuaState()}, undo_{vm.GetLuaState()}
{
    auto luaState = vm.GetLuaState();

    LuaTable<Scope::External> env{luaState, MT_NAME};
    vm.RunFile(scriptPath, env);
    
    execute_ = env.GetLuaWrapper<Script::LuaFunction<Scope::External>>("Execute").value();
    undo_ = env.GetLuaWrapper<Script::LuaFunction<Scope::External>>("Undo").value();
}

std::shared_ptr<ScriptOperation> Script::Type::CreateScript() const
{
    std::shared_ptr<ScriptOperation> s{new ScriptOperation{vm_.GetLuaState(), *this}};

    return s;
}

Operation::Result Script::Type::Execute(::Game& game, const Process::Process& p, LuaTable<Scope::External>& tableRef) const
{
    Operation::Result res{Operation::ERROR};
    
    auto luaState = vm_.GetLuaState();

    // Get Execute function
    execute_.PushInternal();

    // Sets table to function ENV
    tableRef.PushInternal();
    lua_setupvalue(luaState, 1, 1);

    // Push game
    UserData::UserData::PushDataRef<UserData::Game>(luaState, &game);

    // Push process
    UserData::UserData::PushDataCopy<UserData::Process>(luaState, p);

    // Call function
    // TODO: Push prio param beforehand
    auto ret = lua_pcall(luaState, 2, 0, 0);
    if(ret == LUA_OK)
    {
        res = Operation::Result{Operation::SUCCESS};
    }
    else
    {
        std::string error = lua_tostring(luaState, -1);
        lua_pop(luaState, 1);
        
        #ifdef _DEBUG
            std::cout << "Error ocurred while executing Lua Operation: " << error << "\n";
        #endif 
        res = Operation::Result{Operation::ERROR, error};
    }

    return res;
}

void Script::Type::Undo(::Game& game, const Process::Process& p, LuaTable<Scope::External>& tableRef) const
{
    
    auto luaState = vm_.GetLuaState();

    // Get Execute function
    undo_.PushInternal();

    // Sets table to function ENV
    tableRef.PushInternal();
    lua_setupvalue(luaState, 1, 1);

    // Push game
    UserData::UserData::PushDataRef<UserData::Game>(luaState, &game);

    // Push process
    UserData::UserData::PushDataCopy<UserData::Process>(luaState, p);

    // Call function
    // TODO: Push prio param beforehand
    auto ret = lua_pcall(luaState, 2, 0, 0);
    if(ret == LUA_OK)
    {
    }
    else
    {
        std::string error = lua_tostring(luaState, -1);
        lua_pop(luaState, 1);
        
        #ifdef _DEBUG
            std::cout << "Error ocurred while undoing Lua Operation: " << error << "\n";
        #endif 
    }
}
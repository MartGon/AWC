#include <Script/ScriptType.h>
#include <Script/Game.h>
#include <Script/UserData/Game.h>

#include <Script/Wrappers/LuaVM.h>
#include <Script/Wrappers/LuaTable.h>

#include <AWC/AWCException.h>

#ifdef _DEBUG
    #include <iostream>
#endif

using namespace Script;

Script::Type::Type(LuaVM& vm, std::string scriptPath) : scriptPath_{scriptPath}, vm_{vm}, funcs_{LoadFuncs(vm, scriptPath)}
{
    vm.Pop(3); // Pop table and the 2 functions
}

std::array<LuaFunction, 2> Script::Type::LoadFuncs(LuaVM& vm, std::string scriptPath)
{
    auto luaState = vm.GetLuaState();

    LuaTable env{luaState};
    vm.RunFile(scriptPath, env);
    env.PushLuaTable();
    
    auto exType = lua_getfield(luaState, -1, "Execute"); // Table on Top
    auto undoType = lua_getfield(luaState, -2, "Undo"); // Table right below Execute function

    return std::array<LuaFunction, 2>{{ {luaState, -1}, {luaState, -2} }}; // Avoid call to destructor by Copy elision
}

std::shared_ptr<ScriptOperation> Script::Type::CreateScript() const
{
    std::shared_ptr<ScriptOperation> s{new ScriptOperation{vm_.GetLuaState(), *this}};

    return s;
}

Operation::Result Script::Type::Execute(::Game& game, uint8_t prio, LuaTable& tableRef) const
{
    Operation::Result res{Operation::ERROR};
    
    auto luaState = vm_.GetLuaState();

    // Get Execute function
    funcs_[EXECUTE].PushFunction();

    // Get table
    tableRef.PushLuaTable();

    // Sets table to function ENV
    lua_setupvalue(luaState, 1, 1);
    UserData::UserData::PushDataRef<UserData::Game>(luaState, &game);

    // Call function
    // TODO: Push prio param beforehand
    auto ret = lua_pcall(luaState, 1, 0, 0);
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
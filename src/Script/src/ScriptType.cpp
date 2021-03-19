#include <Script/ScriptType.h>
#include <Script/Game.h>
#include <Script/UserData/Game.h>

#include <Script/LuaVM.h>
#include <Script/LuaTable.h>

#include <AWC/AWCException.h>

#ifdef _DEBUG
    #include <iostream>
#endif

using namespace Script;

Script::Type::Type(LuaVM& vm, std::string scriptPath) : scriptPath_{scriptPath}, vm_{vm}, executeRef_{-1}, undoRef_{-1}
{
    auto luaState = vm_.GetLuaState();

    LuaTable env{luaState};
    vm.RunFile(scriptPath, env);
    env.PushLuaTable();
    
    // TODO: Create LuaFunction wrapper;
    auto exType = lua_getfield(luaState, -1, "Execute"); // Table on Top
    auto undoType = lua_getfield(luaState, -2, "Undo"); // Table right below Execute function

    // Get a reference to both functions
    // TODO: Note, could get a reference to ENV table instead. Then call through that table
    if(exType == LUA_TFUNCTION && undoType == LUA_TFUNCTION)
    {
        undoRef_ = luaL_ref(luaState, LUA_REGISTRYINDEX);
        executeRef_ = luaL_ref(luaState, LUA_REGISTRYINDEX);
    }
    else
    {
        throw AWCException{"Execute and Undo vars must be Lua functions"};
    }

    vm.Pop(1); // Pop table
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
    auto type = lua_rawgeti(luaState, LUA_REGISTRYINDEX, executeRef_);
    if(type == LUA_TFUNCTION)
    {
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
    }
    else
        throw AWCException("Could not find stored execute function");

    return res;
}
#include <Script/ScriptType.h>
#include <Script/ScriptGame.h>
#include <Script/UserData/Game.h>

#include <AWC/AWCException.h>

using namespace Script;

Script::Type::Type(lua_State* luaState, std::string scriptPath) : scriptPath_{scriptPath}, luaState_{luaState}, executeRef_{-1}, undoRef_{-1}
{
    auto res = luaL_loadfile(luaState, scriptPath.c_str());
    if(res == LUA_OK)
    {
        // Creates ENV table and gets a ref
        lua_newtable(luaState);
        lua_pushvalue(luaState, -1); // Push a copy for later

        // Set ENV table, avoid polluting global scope
        auto ret = lua_setupvalue(luaState, 1, 1); // The first table ref is popped
        lua_pushvalue(luaState, 1); // Push a copy of the function, for the call
        if(LUA_OK == lua_pcall(luaState, 0, 0, 0))
        {
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
        }
        else
        {
            // TODO: Print the Lua stack
            std::string err = lua_tostring(luaState, -1);
            throw AWCException{"Lua Runtime error: " + err};
        }

        // Pops the ENV table from the stack
        lua_pop(luaState, 1);

        // Pops the lua script function;
        lua_pop(luaState, 1);
    }
    else
    {
        std::string prefix = (res == LUA_ERRSYNTAX ? "Lua Syntax" : "Out of memory") + std::string(" error: ");
        std::string errMsg = std::string(lua_tostring(luaState, -1));
        std::string msg = prefix + errMsg;

        // Pops the lua error msg;
        lua_pop(luaState, 1);

        throw AWCException{msg};
    }
}

std::shared_ptr<ScriptOperation> Script::Type::CreateScript()
{
    std::shared_ptr<ScriptOperation> s{new ScriptOperation{luaState_, *this}};

    return s;
}

Operation::Result Script::Type::Execute(::Game& game, uint8_t prio, int tableRef)
{
    Operation::Result res{Operation::ERROR};
    
    auto luaState = luaState_;

    // Get Execute function
    auto type = lua_rawgeti(luaState, LUA_REGISTRYINDEX, executeRef_);
    if(type == LUA_TFUNCTION)
    {
        // Get table
        type = lua_rawgeti(luaState, LUA_REGISTRYINDEX, tableRef);
        if(type == LUA_TTABLE)
        {
            // Sets table to function ENV
            lua_setupvalue(luaState, 1, 1);
            UserData::Game::PushLight(luaState, &game);

            // Call function
            // TODO: Push game and prio params beforehand
            auto ret = lua_pcall(luaState, 1, 0, 0);
            if(ret == LUA_OK)
            {
                res = Operation::Result{Operation::SUCCESS};
            }
            else
            {
                std::string error = lua_tostring(luaState, -1);
                throw AWCException(error);
            }
        }
    }

    return res;
}
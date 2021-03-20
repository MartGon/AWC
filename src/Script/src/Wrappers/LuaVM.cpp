#include <Script/Wrappers/LuaVM.h>

#include <AWC.h>

using namespace Script;

void LuaVM::Pop(int amount)
{
    lua_pop(ls.L, amount);
} 

unsigned int LuaVM::Call(int nargs, int nresults, int msgh)
{
    auto res = lua_pcall(ls.L, nargs, nresults, msgh);
    if(res != LUA_OK)
    {
        // TODO: Print the Lua stack
        std::string err = lua_tostring(ls.L, -1);
        Pop(1); // Pop error string
        throw AWCException{"Lua Runtime error: " + err};
    }

    return res;
}

unsigned int LuaVM::LoadFile(std::string path)
{
    auto res = luaL_loadfile(ls.L, path.c_str());

    if(res != LUA_OK)
    {
        std::string prefix = (res == LUA_ERRSYNTAX ? "Lua Syntax" : "Out of memory") + std::string(" error: ");
        std::string errMsg = std::string(lua_tostring(ls.L, -1));
        std::string msg = prefix + errMsg;

        // Pops the lua error msg;
        Pop(1);

        throw AWCException{msg};
    }

    return res;
}

unsigned int LuaVM::RunFile(std::string path)
{
    auto res = LoadFile(path);
    res = Call(0, 0, 0);
    Pop(1); // Pop file function

    return res;
}

unsigned int LuaVM::RunFile(std::string path, LuaTable& env)
{
    auto res = LoadFile(path);
    
    // Set ENV table, avoid polluting global scope
    env.PushInternal();
    auto ret = lua_setupvalue(ls.L, 1, 1); // f._ENV = env

    res = Call(0, 0, 0);

    return res;
}
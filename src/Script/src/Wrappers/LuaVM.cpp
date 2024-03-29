#include <Script/Wrappers/LuaVM.h>

#include <AWC.h>

using namespace Script;

int LuaVM::GetTop()
{
    return lua_gettop(ls_.L);
}

void LuaVM::Pop(int amount)
{
    lua_pop(ls_.L, amount);
} 

unsigned int LuaVM::Call(int nargs, int nresults, int msgh)
{
    auto res = lua_pcall(ls_.L, nargs, nresults, msgh);
    if(res != LUA_OK)
    {
        // TODO: Print the Lua stack
        std::string err = lua_tostring(ls_.L, -1);
        Pop(1); // Pop error string
        throw AWC::Exception{"Lua Runtime error: " + err};
    }

    return res;
}

unsigned int LuaVM::LoadFile(std::string path)
{
    auto res = luaL_loadfile(ls_.L, path.c_str());

    if(res != LUA_OK)
    {
        std::string prefix = (res == LUA_ERRSYNTAX ? "Lua Syntax" : "Out of memory") + std::string(" error: ");
        std::string errMsg = std::string(lua_tostring(ls_.L, -1));
        std::string msg = prefix + errMsg;

        // Pops the lua error msg;
        Pop(1);

        throw AWC::Exception{msg};
    }

    return res;
}

unsigned int LuaVM::RunFile(std::string path)
{
    auto res = LoadFile(path);
    res = Call(0, 0, 0);

    return res;
}

unsigned int LuaVM::RunFile(std::string path, LuaTable<Scope::External>& env)
{
    auto res = LoadFile(path);
    
    // Set ENV table, avoid polluting global scope
    env.PushInternal();
    auto ret = lua_setupvalue(ls_.L, 1, 1); // f._ENV = env

    res = Call(0, 0, 0);

    return res;
}

void LuaVM::AppendToPath(std::filesystem::path path)
{
    auto pTable = global_.GetLuaWrapper<Script::LuaTable<Script::Scope::External>>("package");
    auto luaPath = pTable->Get<std::string>("path");
    auto newLuaPath = luaPath + ";" + path.string() + "?.lua";
    pTable->Set("path", newLuaPath);
}
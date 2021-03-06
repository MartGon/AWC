#include <Script.h>

namespace Test::Script
{
    struct TestScript
    {
        TestScript(std::string path, ::Script::Game& sGame) 
            : path{path}, sGame{sGame}, sType{sGame.CreateScriptType(path)}, ref{sGame.CreateScript(sType)}
        {

        }

        inline ::Script::LuaTable& lt()
        {
            return sGame.GetScriptTable(ref);
        }

        std::string path;
        ::Script::Game& sGame;
        unsigned int sType;
        unsigned int ref;
    };

    std::string GetUserDataPath();
}
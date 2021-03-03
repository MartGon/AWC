#include <Script.h>

namespace Test::Script::UserData
{
    struct TestScript
    {
        TestScript(std::string path, ::Script::Game& sGame) : path{path}, sGame{sGame}
        {
            auto sType = sGame.CreateScriptType(path);
            ref = sGame.CreateScript(sType);
        }

        inline ::Script::LuaTable& lt()
        {
            return sGame.GetScriptTable(ref);
        }

        std::string path;
        ::Script::Game& sGame;
        int ref;
    };

    std::string GetUserDataPath();
}
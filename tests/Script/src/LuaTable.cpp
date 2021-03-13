#include "doctest.h"

#include <AWC.h>
#include <Script.h>

#include <Test/Script/Script.h>

TEST_CASE("LuaTable")
{
    Script::Game sGame;
    auto& game = sGame.GetGame();

    std::string path = Test::Script::GetUserDataPath() + "/TilePatternDesc/New.lua";
    Test::Script::TestScript t(path, sGame);

    auto& sTable = t.lt();

    sGame.PushScript(t.ref);
    game.Run();
}
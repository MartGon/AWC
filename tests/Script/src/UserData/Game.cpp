#include "doctest.h"

#include <AWC.h>
#include <Script.h>

TEST_CASE("Game userdata")
{
    Script::Game sGame;
    auto& game = sGame.GetGame();

    Map map{5, 5};
    TileType grassType{0, "Grass"};
    MapUtils::FillMap(map, grassType);

    game.AddMap(map);

    SUBCASE("GetMapCount")
    {
        std::string path = std::string(SCRIPTS_DIR) + "UserData/Game/GetMapCount.lua";
        auto sType = sGame.CreateScriptType(path);
        auto s = sGame.CreateScript(sType);
        auto& sTable = sGame.GetScriptTable(s);
        sGame.PushScript(s);
        game.Run();

        auto value = sTable.GetInt("value");
        CHECK(value == game.GetMapCount());
    }
}
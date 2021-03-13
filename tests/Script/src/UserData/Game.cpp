#include "doctest.h"

#include <AWC.h>
#include <Script.h>

#include <Test/Script/Script.h>

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
        Test::Script::TestScript t(path, sGame);
        sGame.PushScript(t.ref);
        game.Run();

        auto value = t.lt().Get<int>("value");
        CHECK(value == game.GetMapCount());
    }
    SUBCASE("GetMap")
    {
        std::string path = std::string(SCRIPTS_DIR) + "UserData/Game/GetMap.lua";
        Test::Script::TestScript t(path, sGame);
        auto& sTable = t.lt();
        sTable.Set("mapIndex", 0);
        sGame.PushScript(t.ref);
        game.Run();

        auto value = sTable.Get<int>("value");
        CHECK(value == 10);
    }
}
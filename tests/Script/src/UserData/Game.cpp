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

    SUBCASE("CreateMap")
    {
        std::string path = std::string(SCRIPTS_DIR) + "UserData/Game/CreateMap.lua";
        sGame.RunConfig(path);

        CHECK(game.GetMapCount() == 4);
        
        auto& map = game.GetMap(1);
        CHECK(map.GetSize().x == 8);
        CHECK(map.GetSize().y == 8);

        map = game.GetMap(2);
        CHECK(map.GetSize().x == 12);
        CHECK(map.GetSize().y == 12);

        map = game.GetMap(3);
        CHECK(map.GetSize().x == 16);
        CHECK(map.GetSize().y == 16);
    }
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
    SUBCASE("CreatePlayer")
    {
        std::string path = std::string(SCRIPTS_DIR) + "UserData/Game/CreatePlayer.lua";
        sGame.RunConfig(path);

        auto& gTable = sGame.GetLuaVM().GetGlobalTable();
        CHECK(gTable.Get<int>("playerId") == 0);
        CHECK(game.GetPlayerCount() == 1);
    }
}
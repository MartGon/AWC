#include "doctest.h"

#include <AWC.h>
#include <Script.h>

#include <Test/Script/Script.h>
#include <Tests/AWC/UnitTest.h>

TEST_CASE("Map userdata")
{
    Script::Game sGame;
    auto& game = sGame.GetGame();

    Map map{5, 5};
    TileType grassType{0, "Grass"};
    MapUtils::FillMap(map, grassType);

    Player p{0, 0, 1000};
    game.AddPlayer(p);

    auto soldierType = UnitTest::CreateSoldierType();
    auto soldier = soldierType.CreateUnit(p);

    game.AddMap(map);

    game.AddUnit(soldier, {2, 0}, 0);

    SUBCASE("GetUnit")
    {
        std::string path = Test::Script::GetUserDataPath() + "/Map/GetUnit.lua";
        Test::Script::TestScript t(path, sGame);
        
        auto L = sGame.GetLuaState();

        auto& sTable = t.lt();
        sTable.SetRawData("map", Script::UserData::Map::MT_NAME, &game.GetMap(0));
        sTable.SetGCData("origin", Script::UserData::Vector2::MT_NAME, Vector2{2, 0});

        sGame.PushScript(t.ref);
        game.Run();

        auto found = sTable.GetInt("found");
        CHECK(found == 0);

        sTable.SetGCData("origin", Script::UserData::Vector2::MT_NAME, Vector2{0, 0});

        sGame.PushScript(t.ref);
        game.Run();
    }
    SUBCASE("RemoveUnit")
    {
        std::string path = Test::Script::GetUserDataPath() + "/Map/RemoveUnit.lua";
        Test::Script::TestScript t(path, sGame);
        
        auto L = sGame.GetLuaState();

        auto& sTable = t.lt();
        auto& mapRef = game.GetMap(0);
        Vector2 origin{0, 0};
        sTable.SetRawData("map", Script::UserData::Map::MT_NAME, &mapRef);
        sTable.SetGCData("origin", Script::UserData::Vector2::MT_NAME, origin);

        CHECK(soldier.use_count() == 2); // Count is 2: Here and Map

        sGame.PushScript(t.ref);
        game.Run();

        auto success = sTable.GetBool("success");
        CHECK(success == true);
        CHECK(mapRef.GetUnit(origin).get() == nullptr);

        CHECK(soldier.use_count() == 2); // Count is now 2: Here and Lua
    }
}
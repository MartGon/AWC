#include "doctest.h"

#include <AWC.h>
#include <Script.h>

#include <Test/Script/Script.h>
#include <Tests/AWC/UnitTest.h>

TEST_CASE("Map userdata")
{
    Script::Game sGame;
    auto& game = sGame.GetGame();
    auto& db = sGame.GetDB();
    auto& tileTypes = db.get<TileType>();

    Map map{5, 5};
    TileType grassType{0, "Grass"};
    MapUtils::FillMap(map, grassType);

    tileTypes.Add(grassType);

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

        auto& sTable = t.lt();
        sTable.SetDataRef<Script::UserData::Map>("map", &game.GetMap(0));
        sTable.SetDataCopy<Script::UserData::Vector2>("origin", Vector2{2, 0});

        sGame.PushDebugScript(t.ref);
        game.Run();

        auto found = sTable.Get<int>("found");
        CHECK(found == 0);

        sTable.SetDataCopy<Script::UserData::Vector2>("origin", Vector2{0, 0});

        sGame.PushDebugScript(t.ref);
        game.Run();
    }
    SUBCASE("RemoveUnit")
    {
        std::string path = Test::Script::GetUserDataPath() + "/Map/RemoveUnit.lua";
        Test::Script::TestScript t(path, sGame);

        auto& sTable = t.lt();
        auto& mapRef = game.GetMap(0);
        Vector2 origin{0, 0};
        sTable.SetDataRef<Script::UserData::Map>("map", &mapRef);
        sTable.SetDataCopy<Script::UserData::Vector2>("origin", origin);

        CHECK(soldier.use_count() == 2); // Count is 2: Here and Map

        sGame.PushDebugScript(t.ref);
        game.Run();

        auto success = sTable.Get<bool>("success");
        CHECK(success == true);
        CHECK(mapRef.GetUnit(origin).get() == nullptr);

        CHECK(soldier.use_count() == 2); // Count is now 2: Here and Lua
    }
    SUBCASE("AddUnit")
    {
        std::string path = Test::Script::GetUserDataPath() + "/Map/AddUnit.lua";
        Test::Script::TestScript t(path, sGame);

        auto& sTable = t.lt();
        auto& mapRef = game.GetMap(0);
        Vector2 origin{0, 0};
        sTable.SetDataRef<Script::UserData::Map>("map", &mapRef);
        sTable.SetDataCopy<Script::UserData::Unit>("unit", soldier);
        sTable.SetDataCopy<Script::UserData::Vector2>("origin", origin);

        sGame.PushDebugScript(t.ref);
        game.Run();

        auto success = sTable.Get<bool>("success");
        CHECK(success == true);
        CHECK(mapRef.GetUnit(origin).get() != nullptr);
    }
    SUBCASE("Fill")
    {
        std::string path = Test::Script::GetUserDataPath("/Map/Fill.lua");
        auto mapId = game.CreateMap(12, 12);

        sGame.RunConfig(path);

        auto map = game.GetMap(mapId);

        CHECK(map.GetTile(0, 0).get() != nullptr);
        CHECK(map.GetTile(0, 0)->GetName() == "Grass");
    }
}
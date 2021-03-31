#include "doctest.h"

#include <AWC.h>
#include <Script.h>

#include <Test/Script/Script.h>
#include <Tests/AWC/UnitTest.h>

TEST_CASE("Database userdata")
{
    Script::Game sGame;
    auto& game = sGame.GetGame();
    auto& db = sGame.GetDB();
    auto& gTable = sGame.GetLuaVM().GetGlobalTable();

    SUBCASE("Get")
    {
        std::string path = Test::Script::GetUserDataPath() + "/Database/Get.lua";
        Test::Script::TestScript t(path, sGame);

        auto& sTable = t.lt();

        sGame.PushDebugScript(t.ref);
        game.Run();

        bool success = sTable.Get<bool>("Success");
        bool successG = sTable.Get<bool>("SuccessG");
        CHECK(success == true);
        CHECK(successG == true);
    }
    SUBCASE("AddUnitType")
    {
        std::string path = Test::Script::GetUserDataPath() + "/Database/AddUnitType.lua";
        Test::Script::TestScript t(path, sGame);

        auto& sTable = t.lt();

        sGame.PushDebugScript(t.ref);
        game.Run();

        
        auto& unitTypes = db.get<UnitType>();
        auto rookType = unitTypes.GetById(0);
        CHECK(unitTypes.GetFreeId() == 1);
        CHECK(rookType->GetName() == "Rook");
        CHECK(rookType->GetId() == 0);
        CHECK(rookType->GetHandlers().size() > 0);
    }
    SUBCASE("GetUnitType")
    {
        std::string path = Test::Script::GetUserDataPath() + "/Database/GetUnitType.lua";
        Test::Script::TestScript t(path, sGame);

        auto& sTable = t.lt();
        auto& unitTypes = db.get<UnitType>();
        unitTypes.Add(UnitTest::CreateSoldierType());

        sGame.PushDebugScript(t.ref);
        game.Run();

        auto soldierType = sTable.GetUserData<Script::UserData::UnitType>("type");

        CHECK(soldierType == unitTypes.GetById(0));
    }
    SUBCASE("AddTileType")
    {
        std::string path = Test::Script::GetUserDataPath() + "/Database/AddTileType.lua";

        sGame.RunConfig(path);

        auto& tileTypes = db.get<::TileType>();
        auto tt = tileTypes.GetById(0);

        CHECK(tt->GetId() == 0);
        CHECK(tt->GetName() == "Grass");
    }
    SUBCASE("GetTileType")
    {
        std::string path = Test::Script::GetUserDataPath("/Database/GetTileType.lua");

        auto& tileTypes = db.get<::TileType>();
        tileTypes.Add(::TileType(tileTypes.GetFreeId(), "Grass"));

        sGame.RunConfig(path);

        CHECK(gTable.GetUserData<Script::UserData::TileType>("tileType") == tileTypes.GetById(0));
    }
}
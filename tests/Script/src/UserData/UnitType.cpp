#include "doctest.h"

#include <AWC.h>
#include <Script.h>

#include <Test/Script/Script.h>
#include <Tests/AWC/UnitTest.h>


TEST_CASE("UnitType")
{
    Script::Game sGame;
    auto& game = sGame.GetGame();
    auto& gTable = sGame.GetLuaVM().GetGlobalTable();

    SUBCASE("CreateUnit")
    {
        auto& db = sGame.GetDB();
        auto& unitTypes = db.get<::UnitType>();
        unitTypes.Add(UnitTest::CreateSoldierType());

        auto playerId = game.AddPlayer(0);
        auto& owner = game.GetPlayer(playerId);
        gTable.SetDataRef<Script::UserData::Player>("owner", &owner);

        auto path = Test::Script::GetUserDataPath("UnitType/CreateUnit.lua");
        sGame.RunConfig(path);

        auto unit = *gTable.GetUserData<Script::UserData::Unit>("unit");

        CHECK(unit.get() != nullptr);
        CHECK(unit->GetOwner().GetId() == owner.GetId());
        CHECK(unit->GetName() == "Soldier");
        CHECK(unit->GetGUID().id == 0);
    }
}
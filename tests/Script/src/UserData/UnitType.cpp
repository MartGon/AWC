#include "doctest.h"

#include <AWC.h>
#include <Script.h>

#include <Test/Script/Script.h>
#include <Tests/AWC/UnitTest.h>

bool handled = false;

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

        auto playerId = game.AddPlayer(0, 0);
        auto owner = game.GetPlayer(playerId);
        gTable.SetDataRef<Script::UserData::Player>("owner", owner.get());

        auto path = Test::Script::GetUserDataPath("UnitType/CreateUnit.lua");
        sGame.RunConfig(path);

        auto unit = *gTable.GetUserData<Script::UserData::Unit>("unit");

        CHECK(unit.get() != nullptr);
        CHECK(unit->GetOwner()->GetId() == owner->GetId());
        CHECK(unit->GetName() == "Soldier");
        CHECK(unit->GetGUID().id == 0);
    }
    SUBCASE("AddEventHandler")
    {
        auto& db = sGame.GetDB();
        auto& unitTypes = db.get<::UnitType>();
        unitTypes.Add(UnitTest::CreateSoldierType());

        auto playerId = game.AddPlayer(0, 0);
        auto owner = game.GetPlayer(playerId);
        gTable.SetDataRef<Script::UserData::Player>("owner", owner.get());

        Script::LuaFunction<Script::Scope::External> f(sGame.GetLuaVM().GetLuaState(), [](lua_State* state) -> int
        {
            handled = true;
            return 3;
        });
        gTable.SetLuaWrapper<Script::LuaFunction<Script::Scope::External>>("Check", f);

        auto path = Test::Script::GetUserDataPath("UnitType/AddEventHandler.lua");
        sGame.RunConfig(path);

        bool executed = false;
        auto custom = std::make_shared<::Operation::Custom>([&executed](Game& g)
        {
            executed = true;
        });
        game.PushUntriggered(custom);
        game.Run();

        CHECK(executed == true);
        CHECK(handled == true);
    }
}
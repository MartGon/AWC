#include "doctest.h"

#include <AWC.h>
#include <Script.h>

#include <Test/Script/Script.h>

TEST_CASE("Player UserData")
{
    Script::Game sGame;
    auto& game = sGame.GetGame();
    auto& gTable = sGame.GetLuaVM().GetGlobalTable();
    
    SUBCASE("GetTeamId")    
    {
        auto path = Test::Script::GetUserDataPath("Player/GetTeamId.lua");    
        auto playerId = game.AddPlayer(0, 0);

        gTable.SetDataRef<Script::UserData::Player>("player", game.GetPlayer(playerId).get());

        sGame.RunConfig(path);

        CHECK(gTable.Get<unsigned int>("teamId") == 0);
    }
}
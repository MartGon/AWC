#include "doctest.h"

#include <AWC.h>
#include <Script.h>

#include <Test/Script/Script.h>

TEST_CASE("TileType")
{
    Script::Game sGame;
    auto& db = sGame.GetDB();
    auto& gTable = sGame.GetLuaVM().GetGlobalTable();

    SUBCASE("CreateTile")
    {
        auto& db = sGame.GetDB();
        auto& tileTypes = db.get<::TileType>();
        tileTypes.Add(TileType{0, "Grass"});

        auto path = Test::Script::GetUserDataPath("TileType/CreateTile.lua");
        sGame.RunConfig(path);

        auto tile = *gTable.GetUserData<Script::UserData::Tile>("tile");

        CHECK(tile.get() != nullptr);
        CHECK(tile->GetId() == 0);
        CHECK(tile->GetName() == "Grass");
    }
}
#include "doctest.h"

#include <AWCSer/AWCSer.h>
#include <AWCSer/TilePatternDescSer.h>
#include <AWCSer/JsonUtils.h>

#include <AWC/TileType.h>
#include <AWC/Tile.h>
#include <AWC/Map.h>

#include <AWCSerTest/AWCSerTest.h>

#include <TilePatternCompTest.h>


#include <iostream>

TEST_CASE("TileType")
{
    SUBCASE("Load from data")
    {
        Json data{{"id", 0}, {"name", "Grass"}};
        TileType tileType = AWCSer::LoadTileType(data);
        
        CHECK(tileType.GetId() == 0);
        CHECK(tileType.GetName() == "Grass");
    }
    SUBCASE("Load from file")
    {
        const std::string filename = "TileType.json";
        Json data = JsonUtils::GetJsonFromFile(filename, AWCSerTest::GetJsonFolderPath());
        TileType tileType = AWCSer::LoadTileType(data);
        
        CHECK(tileType.GetId() == 0);
        CHECK(tileType.GetName() == "Grass");
    }
}

TEST_CASE("Tile")
{
    // Prepare repo
    Repository<TileType> tileTypeRepo;
    TileType tileType{0, "Grass"};
    tileTypeRepo.Add(tileType.GetId(), tileType);

    SUBCASE("Load from json")
    {
        Json data{{"tileTypeId", 0}, {"guid", 0}};
        TilePtr tile = AWCSer::LoadTile(data, tileTypeRepo);

        CHECK(tile.get() != nullptr);
        CHECK(tile->GetId() == 0);
        CHECK(tile->GetName() == "Grass");
    }
    SUBCASE("Loading from file")
    {
        Json data = JsonUtils::GetJsonFromFile("Tile.json", AWCSerTest::GetJsonFolderPath());
        TilePtr tile = AWCSer::LoadTile(data, tileTypeRepo);

        CHECK(tile.get() != nullptr);
        CHECK(tile->GetId() == 0);
        CHECK(tile->GetName() == "Grass");
    }
}

std::string AWCSerTest::GetJsonFolderPath()
{
    return PROJECT_DIR + std::string("/resources/AWCSer/");
}
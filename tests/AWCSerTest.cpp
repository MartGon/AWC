#include "doctest.h"

#include <AWCSer/AWCSer.h>

#include <AWC/TileType.h>
#include <AWC/Tile.h>

#include <AWCSerTest.h>

#include <fstream>
#include <iostream>

const std::string JSON_FILES_PATH = "/resources/AWCSer/";

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
        Json data = AWCSerTest::GetJsonFromFile(filename);
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
        Json data = AWCSerTest::GetJsonFromFile("Tile.json");
        TilePtr tile = AWCSer::LoadTile(data, tileTypeRepo);

        CHECK(tile.get() != nullptr);
        CHECK(tile->GetId() == 0);
        CHECK(tile->GetName() == "Grass");
    }
}

Json AWCSerTest::GetJsonFromFile(std::string filename)
{
    const std::string path = PROJECT_DIR + JSON_FILES_PATH + filename;

    std::ifstream file{path};
    CHECK(file.good() == true);

    Json data = Json::parse(file);

    return data;
}
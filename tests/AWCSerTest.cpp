#include "doctest.h"

#include <AWCSer/AWCSer.h>

#include <AWC/TileType.h>

#include <fstream>
#include <iostream>

TEST_CASE("Serialization from string")
{
    SUBCASE("TileType")
    {
        Json data{{"id", 0}, {"name", "Grass"}};
        TileType tileType = AWCSer::LoadTileType(data);
        
        CHECK(tileType.GetId() == 0);
        CHECK(tileType.GetName() == "Grass");
    }
}

const std::string JSON_FILES_PATH = "/resources/AWCSer/";

TEST_CASE("Serialization from file")
{
    SUBCASE("TileType")
    {
        const std::string filename = "TileType.json";
        const std::string path = PROJECT_DIR + JSON_FILES_PATH + filename;

        std::ifstream file{path};
        CHECK(file.good() == true);

        Json data = Json::parse(file);
        TileType tileType = AWCSer::LoadTileType(data);
        
        CHECK(tileType.GetId() == 0);
        CHECK(tileType.GetName() == "Grass");
    }
}
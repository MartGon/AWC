#include "doctest.h"

#include <AWCSer/AWCSer.h>

#include <AWC/TileType.h>
#include <AWC/Tile.h>

#include <AWC/TilePatternDesc.h>

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

TEST_CASE("TilePatternDesc")
{
    SUBCASE("From json")
    {
        Json data{{"type", 0}, {"originDirections", 
        {
            {
                {"x", 1},
                {"y", 0}
            },
            {
                {"x", 0},
                {"y", 1}
            },
            {
                {"x", -1},
                {"y", 0}
            },
            {
                {"x", 0},
                {"y", -1}
            }
        }}};
        Directions dirs = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

        TilePatternDescPtr tpdp = AWCSer::LoadTilePatternDesc(data);
        
        Directions tpDirs = tpdp->GetOriginDirections();
        for(auto dir : dirs)
            CHECK(VectorUtils::IsInside(tpDirs, dir));
    }
    SUBCASE("From file")
    {
        Directions dirs = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
        Json data = AWCSerTest::GetJsonFromFile("TilePatternDesc.json");
        TilePatternDescPtr tpdp = AWCSer::LoadTilePatternDesc(data);

        Directions tpDirs = tpdp->GetOriginDirections();
        for(auto dir : dirs)
            CHECK(VectorUtils::IsInside(tpDirs, dir));
    }
}

TEST_CASE("TilePatternDesc Rook")
{
    Vector2 e = {1, 0};
    Vector2 w = {-1, 0};
    Vector2 n = {0, 1};
    Vector2 s = {0, -1};
    Directions dirs = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    DirectionsTable lockedDirTableR = {
        {e, {e}},
        {w, {w}},
        {n, {n}},
        {s, {s}}
    };

    SUBCASE("From json")
    {
        Json data{{"type", 0}, 
        {"originDirections", 
            {
                {
                    {"x", 1},
                    {"y", 0}
                },
                {
                    {"x", 0},
                    {"y", 1}
                },
                {
                    {"x", -1},
                    {"y", 0}
                },
                {
                    {"x", 0},
                    {"y", -1}
                }
            }   
        },
        { "directionsTable",
            {
                {"type", 0},
                {"table", 
                    {
                        {
                            {"indexDir",
                                {
                                    {"x", 1},
                                    {"y", 0}
                                }
                            },
                            {"directions",
                                {
                                    {
                                        {"x", 1},
                                        {"y", 0}
                                    }
                                }
                            }
                        },
                        {
                            {"indexDir",
                                {
                                    {"x", -1},
                                    {"y", 0}
                                }
                            },
                            {"directions",
                                {
                                    {
                                        {"x", -1},
                                        {"y", 0}
                                    }
                                }
                            }
                        },
                        {
                            {"indexDir",
                                {
                                    {"x", 0},
                                    {"y", 1}
                                }
                            },
                            {"directions",
                                {
                                    {
                                        {"x", 0},
                                        {"y", 1}
                                    }
                                }
                            }
                        },
                        {
                            {"indexDir",
                                {
                                    {"x", 0},
                                    {"y", -1}
                                }
                            },
                            {"directions",
                                {
                                    {
                                        {"x", 0},
                                        {"y", -1}
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    };
        
        TilePatternDescPtr tpdp = AWCSer::LoadTilePatternDesc(data);
        
        Directions tpDirs = tpdp->GetOriginDirections();

        for(auto dir : dirs)
            CHECK(VectorUtils::IsInside(tpDirs, dir));
        
        for(auto dir : tpDirs)
            lockedDirTableR.at(dir) == tpdp->GetLockedDirections(dir);

    }
    SUBCASE("From file")
    {
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
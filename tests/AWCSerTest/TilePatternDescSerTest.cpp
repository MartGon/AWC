#include "doctest.h"

#include <AWCSer/AWCSer.h>
#include <AWCSer/TilePatternDescSer.h>

#include <AWC/TilePatternDesc.h>
#include <AWC/TilePatternConstraints.h>
#include <AWC/CostTable.h>

#include <AWCSerTest/AWCSerTest.h>
#include <TilePatternCompTest.h>

#include <fstream>
#include <iostream>


TEST_CASE("TilePatternDesc")
{
    Directions dirs = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

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

        TilePatternDescPtr tpdp = TilePatternDescSer::LoadTilePatternDesc(data);
        
        Directions tpDirs = tpdp->GetOriginDirections();
        for(auto dir : dirs)
            CHECK(VectorUtils::IsInside(tpDirs, dir));
    }
    SUBCASE("From file")
    {
        Json data = AWCSerTest::GetJsonFromFile("TilePatternDescBase.json");
        TilePatternDescPtr tpdp = TilePatternDescSer::LoadTilePatternDesc(data);

        Directions tpDirs = tpdp->GetOriginDirections();
        for(auto dir : dirs)
            CHECK(VectorUtils::IsInside(tpDirs, dir));
    }
}

TEST_CASE("TilePatternDesc Rook by Locked")
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
        
        TilePatternDescPtr tpdp = TilePatternDescSer::LoadTilePatternDesc(data);
        Directions tpDirs = tpdp->GetOriginDirections();

        for(auto dir : dirs)
            CHECK(VectorUtils::IsInside(tpDirs, dir));
        
        for(auto dir : tpDirs)
            lockedDirTableR.at(dir) == tpdp->GetLockedDirections(dir);

    }
    SUBCASE("From file")
    {
        Json data = AWCSerTest::GetJsonFromFile("TilePatternDescBaseLocked.json");
        TilePatternDescPtr tpdp = TilePatternDescSer::LoadTilePatternDesc(data);
        Directions tpDirs = tpdp->GetOriginDirections();

        for(auto dir : dirs)
            CHECK(VectorUtils::IsInside(tpDirs, dir));
        
        for(auto dir : tpDirs)
            lockedDirTableR.at(dir) == tpdp->GetLockedDirections(dir);
    }
}

TEST_CASE("TilePatternDesc Rook by Exclusive")
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
                                        {"x", -1},
                                        {"y", 0}
                                    },
                                    {
                                        {"x", 0},
                                        {"y", 1}
                                    },
                                    {
                                        {"x", 0},
                                        {"y", -1}
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
                                        {"x", 1},
                                        {"y", 0}
                                    },
                                    {
                                        {"x", 0},
                                        {"y", 1}
                                    },
                                    {
                                        {"x", 0},
                                        {"y", -1}
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
                                        {"x", -1},
                                        {"y", 0}
                                    },
                                    {
                                        {"x", 1},
                                        {"y", 0}
                                    },
                                    {
                                        {"x", 0},
                                        {"y", -1}
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
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        };
        
        TilePatternDescPtr tpdp = TilePatternDescSer::LoadTilePatternDesc(data);
        Directions tpDirs = tpdp->GetOriginDirections();

        for(auto dir : dirs)
            CHECK(VectorUtils::IsInside(tpDirs, dir));
        
        for(auto dir : tpDirs)
            lockedDirTableR.at(dir) == tpdp->GetLockedDirections(dir);

    }
    SUBCASE("From file")
    {
        Json data = AWCSerTest::GetJsonFromFile("TilePatternDescBaseExclusive.json");
        TilePatternDescPtr tpdp = TilePatternDescSer::LoadTilePatternDesc(data);
        Directions tpDirs = tpdp->GetOriginDirections();

        for(auto dir : dirs)
            CHECK(VectorUtils::IsInside(tpDirs, dir));
        
        for(auto dir : tpDirs)
            lockedDirTableR.at(dir) == tpdp->GetLockedDirections(dir);
    }
}

TEST_CASE("TilePatternDescCompUnion")
{
    Repository<TilePatternDescIPtr> repo;
    auto rookDesc = TilePatternCompTest::GetRookDesc();
    auto bishopDesc = TilePatternCompTest::GetBishopDesc();
    repo.Add(0, rookDesc);
    repo.Add(1, bishopDesc);

    SUBCASE("From json")
    {
        Json data{
                {"type", 1}, {"name", "Queen"}, {"subType", 0}, 
                {"tpdA", 0}, {"tpdB", 1}
            };
        auto queenDesc = TilePatternDescSer::LoadTilePatternDescI(data, repo);

        TilePatternCompTest::CheckQueenDesc(queenDesc);
    }
    SUBCASE("From file")
    {
        Json data = AWCSerTest::GetJsonFromFile("TilePatternDescComp.json");
        auto queenDesc = TilePatternDescSer::LoadTilePatternDescI(data, repo);

        TilePatternCompTest::CheckQueenDesc(queenDesc);
    }
}
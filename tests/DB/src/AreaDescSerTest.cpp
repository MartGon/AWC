#include "doctest.h"

#include <DB/AWCSer.h>
#include <DB/AreaDescSer.h>

#include <AWC/Area/AreaDesc.h>
#include <AWC/Area/AreaConstraints.h>
#include <AWC/CostTable.h>

#include <Tests/DB/AWCSerTest.h>
#include <DB/JsonUtils.h>
#include <Tests/AWC/AreaCompTest.h>

#include <fstream>
#include <iostream>


TEST_CASE("AreaDesc")
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

        AreaDescPtr tpdp = AreaDescSer::LoadAreaDesc(data);
        
        Directions tpDirs = tpdp->GetOriginDirections();
        for(auto dir : dirs)
            CHECK(VectorUtils::IsInside(tpDirs, dir));
    }
    SUBCASE("From file")
    {
        Json data = JsonUtils::GetJsonFromFile("AreaDescBase.json", RESOURCES_DIR);
        AreaDescPtr tpdp = AreaDescSer::LoadAreaDesc(data);

        Directions tpDirs = tpdp->GetOriginDirections();
        for(auto dir : dirs)
            CHECK(VectorUtils::IsInside(tpDirs, dir));
    }
}

TEST_CASE("AreaDesc Rook by Locked")
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
        
        AreaDescPtr tpdp = AreaDescSer::LoadAreaDesc(data);
        Directions tpDirs = tpdp->GetOriginDirections();

        for(auto dir : dirs)
            CHECK(VectorUtils::IsInside(tpDirs, dir));
        
        for(auto dir : tpDirs)
            lockedDirTableR.at(dir) == tpdp->GetLockedDirections(dir);

    }
    SUBCASE("From file")
    {
        Json data = JsonUtils::GetJsonFromFile("AreaDescBaseLocked.json", RESOURCES_DIR);
        AreaDescPtr tpdp = AreaDescSer::LoadAreaDesc(data);
        Directions tpDirs = tpdp->GetOriginDirections();

        for(auto dir : dirs)
            CHECK(VectorUtils::IsInside(tpDirs, dir));
        
        for(auto dir : tpDirs)
            lockedDirTableR.at(dir) == tpdp->GetLockedDirections(dir);
    }
}

TEST_CASE("AreaDesc Rook by Exclusive")
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
        
        AreaDescPtr tpdp = AreaDescSer::LoadAreaDesc(data);
        Directions tpDirs = tpdp->GetOriginDirections();

        for(auto dir : dirs)
            CHECK(VectorUtils::IsInside(tpDirs, dir));
        
        for(auto dir : tpDirs)
            lockedDirTableR.at(dir) == tpdp->GetLockedDirections(dir);

    }
    SUBCASE("From file")
    {
        Json data = JsonUtils::GetJsonFromFile("AreaDescBaseExclusive.json", RESOURCES_DIR);
        AreaDescPtr tpdp = AreaDescSer::LoadAreaDesc(data);
        Directions tpDirs = tpdp->GetOriginDirections();

        for(auto dir : dirs)
            CHECK(VectorUtils::IsInside(tpDirs, dir));
        
        for(auto dir : tpDirs)
            lockedDirTableR.at(dir) == tpdp->GetLockedDirections(dir);
    }
}

TEST_CASE("AreaDescCompUnion")
{
    Repository<AreaDescIPtr> repo;
    auto rookDesc = AreaCompTest::GetRookDesc();
    auto bishopDesc = AreaCompTest::GetBishopDesc();
    repo.Add(0, rookDesc);
    repo.Add(1, bishopDesc);

    SUBCASE("From json")
    {
        Json data{
                {"type", 1}, {"name", "Queen"}, {"subType", 0}, 
                {"tpdA", 0}, {"tpdB", 1}
            };
        auto queenDesc = AreaDescSer::LoadAreaDescI(data, repo);

        AreaCompTest::CheckQueenDesc(queenDesc);
    }
    SUBCASE("From file")
    {
        Json data = JsonUtils::GetJsonFromFile("AreaDescComp.json", RESOURCES_DIR);
        auto queenDesc = AreaDescSer::LoadAreaDescI(data, repo);

        AreaCompTest::CheckQueenDesc(queenDesc);
    }
}
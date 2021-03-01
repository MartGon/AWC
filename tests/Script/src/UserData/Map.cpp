#include "doctest.h"

#include <AWC.h>
#include <Script.h>

TEST_CASE("Map userdata")
{
    Script::Game sGame;
    auto& game = sGame.GetGame();

    Map map{5, 5};
    TileType grassType{0, "Grass"};
    MapUtils::FillMap(map, grassType);

    game.AddMap(map);

    SUBCASE("GetUnit")
    {
        
    }
}
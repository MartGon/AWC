#include "doctest.h"

#include <AWC/TileType.h>
#include <AWC/Tile.h>

// Size checking
TEST_CASE("Tiles have some properties")
{
    std::string tileName = "Grass";
    TileType grassTileType{tileName};

    SUBCASE("Tiles are of a given type")
    {
        auto grass = grassTileType.CreateTile();
        CHECK(grass->GetName() == tileName);
    }
}
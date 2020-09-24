#include <AWC/Tile.h>
#include <AWC/TileType.h>

Tile::Tile(TileType& tileType) : tileType_{tileType}
{

}

std::string Tile::GetName()
{
    return tileType_.GetName();
}
#include <AWC/Tile.h>
#include <AWC/TileType.h>

Tile::Tile(const TileType& tileType) : tileType_{tileType}
{

}

unsigned int Tile::GetId() const
{
     return tileType_.GetId();
}

std::string Tile::GetName() const
{
    return tileType_.GetName();
}
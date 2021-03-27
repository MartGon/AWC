#include <AWC/Tile.h>
#include <AWC/TileType.h>

Tile::Tile(const TileType& tileType, unsigned int id) : tileType_{tileType}, id_{id}
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
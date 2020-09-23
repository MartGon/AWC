#include <AWC/Tile.h>
#include <AWC/TileType.h>

Tile::Tile(TileType& tileType) : _tileType{tileType}
{

}

std::string Tile::GetName()
{
    return _tileType.GetName();
}
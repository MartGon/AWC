#include <AWC/TileType.h>
#include <AWC/Tile.h>

TileType::TileType(const std::string& name) : _name{name}
{

}

Tile* TileType::CreateTile()
{
    return new Tile(*this);
}

std::string TileType::GetName() const
{
    return _name;
}
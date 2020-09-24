#include <AWC/TileType.h>
#include <AWC/Tile.h>

TileType::TileType(const std::string& name) : name_{name}
{

}

Tile* TileType::CreateTile()
{
    return new Tile(*this);
}

std::string TileType::GetName() const
{
    return name_;
}
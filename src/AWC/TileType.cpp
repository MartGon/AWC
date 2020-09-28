#include <AWC/TileType.h>
#include <AWC/Tile.h>

TileType::TileType(const std::string& name) : name_{name}
{

}

std::shared_ptr<Tile> TileType::CreateTile()
{
    return std::make_shared<Tile>(Tile{*this});
}

std::string TileType::GetName() const
{
    return name_;
}
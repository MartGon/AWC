#include <AWC/TileType.h>
#include <AWC/Tile.h>

TileType::TileType(unsigned int id, std::string name) : id_{id}, name_{name}
{

}

std::shared_ptr<Tile> TileType::CreateTile() const
{
    return std::make_shared<Tile>(Tile{*this});
}

unsigned int TileType::GetId() const
{
    return id_;
}

std::string TileType::GetName() const
{
    return name_;
}
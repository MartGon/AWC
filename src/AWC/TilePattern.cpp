#include <AWC/TilePattern.h>

TilePattern::TilePattern(Vector2 origin, TileGraph tg) : origin_{origin}, tg_{tg}
{
    
}

bool TilePattern::IsTileInPattern(Vector2 dest)
{
    return tg_.NodeExists(dest);
}

unsigned int TilePattern::GetTileCost(Vector2 dest)
{
    return tg_.GetNode(dest).lock()->cost;
}
#pragma once
#include <AWC/AWCusing.h>

#include <Utils/Vector2.h>

class UnitWeapon
{
public:
    UnitWeapon(TilePatternDescIPtr tpd, uint maxRange, uint minRange = 1);

    UnitAttack CalculateUnitAttack(const Map& map, Vector2 origin, Vector2 dest);
    UnitAttack CalculateUnitAttack(const Map& map, Vector2 origin);

private:
    TilePatternDescIPtr tilePatternDesc_;

    TilePatternConstraints GetAttackConstraints();
    
    unsigned int minRange_;
    unsigned int maxRange_; 
};

class UnitAttack
{
    friend class UnitWeapon;
public:

    bool CanAttack(Vector2 dest);
    std::vector<Vector2> GetTilesInRange();

private:
    UnitAttack(TilePatternIPtr tp);

    TilePatternIPtr tp_;
};
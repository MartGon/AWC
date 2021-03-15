#pragma once

class Command;
class CostTable;
class CostTableI;
class FixedCostTable;
class Game;
class Map;
class MovementDescType;
class MovementDesc;
struct Range;
class Player;
struct Position;
struct Process;
class Tile;
class TileType;
class TileGraph;
class TileNode;
class Area;
class AreaI;
class AreaUnion;
class AreaDiff;
class AreaIntersect;
class AreaConstraints;
class AreaDesc;
class AreaDescI;
class TPDStaticRange;
class TPDStaticCost;
class Turn;
class Unit;
class UnitAttack;
class UnitMovement;
class UnitType;
class Weapon;
class WeaponType;

namespace UnitNS
{
    enum Flag : unsigned int;
    enum StatType : unsigned int;
    struct GUID;
}

namespace Operation
{
    class OperationI;
    class Custom;
    class Composed;
    class Move;
    class AntiOperation;
    class StatMod;
    class TakeDmg;
    class UpdateFlag;
    class Attack;
};
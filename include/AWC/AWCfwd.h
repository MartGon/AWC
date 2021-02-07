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
struct Process;
class Tile;
class TileType;
class TileGraph;
class TileNode;
class TilePattern;
class TilePatternI;
class TilePatternUnion;
class TilePatternDiff;
class TilePatternIntersect;
class TilePatternConstraints;
class TilePatternDesc;
class TilePatternDescI;
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
}

namespace Operation
{
    class OperationI;
    enum class Type;
};
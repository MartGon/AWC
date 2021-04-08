SET(SOURCES
  src/Exception.cpp
  src/Command.cpp
  src/CostTable.cpp
  src/Entity.cpp
  src/Event.cpp
  src/Game.cpp
  src/Map.cpp
  src/Range.cpp
  src/Pathfinding.cpp
  src/Player.cpp
  src/Process.cpp
  src/Tile.cpp
  src/TileType.cpp
  src/Turn.cpp

  src/Area/Area.cpp
  src/Area/AreaI.cpp
  src/Area/AreaComp.cpp
  src/Area/AreaConstraints.cpp
  src/Area/AreaDesc.cpp
  src/Area/AreaDescI.cpp
  src/Area/AreaDescComp.cpp
  src/Area/AreaDescDecorator.cpp
  src/Area/Graph.cpp
  src/Area/Node.cpp

  src/Operation/Operation.cpp
  src/Operation/AntiOperation.cpp
  src/Operation/Composed.cpp
  src/Operation/Custom.cpp
  src/Operation/Move.cpp
  src/Operation/StatMod.cpp
  src/Operation/Attack.cpp
  src/Operation/TakeDmg.cpp
  src/Operation/UpdateFlag.cpp

  src/Unit/MovementDescType.cpp
  src/Unit/MovementDesc.cpp
  src/Unit/Unit.cpp
  src/Unit/AttackArea.cpp
  src/Unit/UnitType.cpp
  src/Unit/MoveArea.cpp
  src/Unit/Weapon.cpp
  src/Unit/WeaponType.cpp
)

add_library(AWC
  ${SOURCES}
)

target_link_libraries(AWC Utils)
target_include_directories(AWC PUBLIC include ../Utils/include)

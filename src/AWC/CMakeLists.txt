SET(SOURCES
  src/AWCException.cpp
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

  src/TilePattern/TilePattern.cpp
  src/TilePattern/TilePatternI.cpp
  src/TilePattern/TilePatternComp.cpp
  src/TilePattern/TilePatternConstraints.cpp
  src/TilePattern/TilePatternDesc.cpp
  src/TilePattern/TilePatternDescI.cpp
  src/TilePattern/TilePatternDescComp.cpp
  src/TilePattern/TilePatternDescDecorator.cpp
  src/TilePattern/TileGraph.cpp
  src/TilePattern/TileNode.cpp

  src/Operation/Factory.cpp
  src/Operation/Operation.cpp
  src/Operation/AntiOperation.cpp
  src/Operation/Composed.cpp
  src/Operation/Custom.cpp
  src/Operation/Move.cpp
  src/Operation/StatMod.cpp
  src/Operation/Attack.cpp
  src/Operation/TakeDmg.cpp
  src/Operation/UpdateFlag.cpp
  src/Operation/ScriptType.cpp

  src/Unit/MovementDescType.cpp
  src/Unit/MovementDesc.cpp
  src/Unit/Unit.cpp
  src/Unit/UnitAttack.cpp
  src/Unit/UnitType.cpp
  src/Unit/UnitMovement.cpp
  src/Unit/Weapon.cpp
  src/Unit/WeaponType.cpp
)

add_library(AWC
  ${SOURCES}
)

target_link_libraries(AWC ${LUA_LIB_PATH})
target_include_directories(AWC PUBLIC include ../Utils/include ${LUA_INCLUDE_DIR})

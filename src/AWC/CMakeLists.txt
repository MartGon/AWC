add_library(AWCSource
  AWCException.cpp
  Command.cpp
  CostTable.cpp
  Entity.cpp
  Event.cpp
  Game.cpp
  Map.cpp
  Range.cpp
  Pathfinding.cpp
  Player.cpp
  Process.cpp
  Tile.cpp
  TileType.cpp
  Turn.cpp

  TilePattern/TilePattern.cpp
  TilePattern/TilePatternI.cpp
  TilePattern/TilePatternComp.cpp
  TilePattern/TilePatternConstraints.cpp
  TilePattern/TilePatternDesc.cpp
  TilePattern/TilePatternDescI.cpp
  TilePattern/TilePatternDescComp.cpp
  TilePattern/TilePatternDescDecorator.cpp
  TilePattern/TileGraph.cpp
  TilePattern/TileNode.cpp

  Operation/Factory.cpp
  Operation/Operation.cpp
  Operation/AntiOperation.cpp
  Operation/Composed.cpp
  Operation/Custom.cpp
  Operation/Move.cpp
  Operation/StatMod.cpp
  Operation/Attack.cpp
  Operation/TakeDmg.cpp
  Operation/UpdateFlag.cpp

  Unit/MovementDescType.cpp
  Unit/MovementDesc.cpp
  Unit/Unit.cpp
  Unit/UnitAttack.cpp
  Unit/UnitType.cpp
  Unit/UnitMovement.cpp
  Unit/Weapon.cpp
  Unit/WeaponType.cpp
)

target_link_libraries(AWCSource /home/defu/Libs/lua-5.4.2/src/liblua.a)
target_include_directories(AWCSource PUBLIC ${CMAKE_SOURCE_DIR}/include /home/defu/Libs/lua-5.4.2/src)
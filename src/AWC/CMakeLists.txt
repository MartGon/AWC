add_library(AWCSource
  AWCException.cpp
  Command.cpp
  CostTable.cpp
  Map.cpp
  Tile.cpp
  TileType.cpp
  UnitMovement.cpp
  UnitWeapon.cpp
  Unit.cpp
  UnitType.cpp
  TilePattern.cpp
  TilePatternI.cpp
  TilePatternComp.cpp
  TilePatternConstraints.cpp
  TilePatternDesc.cpp
  TilePatternDescI.cpp
  TilePatternDescComp.cpp
  TilePatternDescDecorator.cpp
  TileGraph.cpp
  TileNode.cpp
)

target_include_directories(AWCSource PUBLIC ${CMAKE_SOURCE_DIR}/include)
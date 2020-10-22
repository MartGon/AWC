add_library(AWCSource 
  Command.cpp
  CostTable.cpp
  Map.cpp
  Tile.cpp
  TileType.cpp
  Unit.cpp
  UnitType.cpp
  TilePattern.cpp
  TilePatternI.cpp
  TilePatternComp.cpp
  TilePatternConstraints.cpp
  TilePatternDescriptor.cpp
  TilePatternDescriptorI.cpp
  TilePatternDescriptorComp.cpp
  TileGraph.cpp
  TileNode.cpp
)

target_include_directories(AWCSource PUBLIC ${CMAKE_SOURCE_DIR}/include)
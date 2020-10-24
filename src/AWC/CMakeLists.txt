add_library(AWCSource
  AWCException.cpp
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
  TilePatternDescriptorDecorator.cpp
  TileGraph.cpp
  TileNode.cpp
)

target_include_directories(AWCSource PUBLIC ${CMAKE_SOURCE_DIR}/include)
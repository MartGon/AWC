add_library(AWCSource 
  Command.cpp
  Map.cpp
  Tile.cpp
  TileType.cpp
  Unit.cpp
  UnitType.cpp
  TilePattern.cpp
  TilePatternDescriptor.cpp
  TileGraph.cpp
  TileNode.cpp
)

target_include_directories(AWCSource PUBLIC ${CMAKE_SOURCE_DIR}/include)
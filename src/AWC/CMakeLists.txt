add_library(AWCSource 
  Command.cpp
  Map.cpp
  Tile.cpp
  TileType.cpp
  Unit.cpp
  UnitType.cpp
  TilePattern.cpp
)

target_include_directories(AWCSource PUBLIC ${CMAKE_SOURCE_DIR}/include)
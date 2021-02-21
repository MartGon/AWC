SET(SOURCES
    src/CommandTest.cpp
    src/CostTableTest.cpp
    src/EventsTest.cpp
    src/GameTest.cpp
    src/MapTest.cpp
    src/TileTest.cpp
    src/UnitTest.cpp
    src/TilePatternTest.cpp
    src/TilePatternCompTest.cpp
    src/TilePatternDescTest.cpp
    src/TilePatternDescDecoratorTest.cpp
    src/TileGraphTest.cpp
)

add_library(AWCTest ${SOURCES})

target_include_directories(AWCTest PUBLIC include
    ${CMAKE_SOURCE_DIR}/src/AWC/include
    ${CMAKE_SOURCE_DIR}/src/Utils/include  
    ${LUA_INCLUDE_DIR}
    ${DOCTEST_INCLUDE_DIR})
target_link_libraries(AWCTest AWC Utils ${LUA_LIB_PATH})
SET(SOURCES

    src/AWCTest.cpp

    src/CommandTest.cpp
    src/CostTableTest.cpp
    src/EventsTest.cpp
    src/GameTest.cpp
    src/MapTest.cpp
    src/TileTest.cpp
    src/UnitTest.cpp
    src/AreaTest.cpp
    src/AreaCompTest.cpp
    src/AreaDescTest.cpp
    src/AreaDescDecoratorTest.cpp
    src/GraphTest.cpp
    src/UnitMovementTest.cpp
)

add_library(AWCTestLib ${SOURCES})
target_include_directories(AWCTestLib PUBLIC include
    ${CMAKE_SOURCE_DIR}/src/AWC/include
    ${CMAKE_SOURCE_DIR}/src/Utils/include  
    ${LUA_INCLUDE_DIR}
    ${DOCTEST_INCLUDE_DIR})
target_link_libraries(AWCTestLib AWC Utils)


add_executable(AWCTest ${SOURCES})
target_include_directories(AWCTest PUBLIC include
    ${CMAKE_SOURCE_DIR}/src/AWC/include
    ${CMAKE_SOURCE_DIR}/src/Utils/include  
    ${LUA_INCLUDE_DIR}
    ${DOCTEST_INCLUDE_DIR})
target_link_libraries(AWCTest AWC Utils)

add_test(NAME RunAWCTest COMMAND AWCTest)
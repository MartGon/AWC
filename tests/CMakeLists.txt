add_executable(AWCTest 
    AWCTest.cpp
    CommandTest.cpp
    CostTableTest.cpp
    EventsTest.cpp
    GameTest.cpp
    MapTest.cpp
    TileTest.cpp
    UnitTest.cpp
    TilePatternTest.cpp
    TilePatternCompTest.cpp
    TilePatternDescTest.cpp
    TilePatternDescDecoratorTest.cpp
    TileGraphTest.cpp

    AWCSerTest/AWCSerTest.cpp
    AWCSerTest/TilePatternDescSerTest.cpp
    AWCSerTest/JsonUtilsTest.cpp

    Scripting/ScriptTest.cpp

    Utils/STLUtilsTest.cpp
)

# target_compile_features(AWCTest PRIVATE cxx_std_20)
target_include_directories(AWCTest PUBLIC ${DOCTEST_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/include include ${LUA_INCLUDE_DIR})
target_link_libraries(AWCTest AWCLibrary AWCSer ${LUA_LIB_PATH})

add_compile_definitions(PROJECT_DIR="${CMAKE_SOURCE_DIR}")
add_compile_definitions(RESOURCES_DIR="${CMAKE_SOURCE_DIR}/resources/AWCSer/")

add_test(NAME BasicTest COMMAND AWCTest)
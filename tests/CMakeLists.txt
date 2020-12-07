add_executable(AWCTest 
    AWCTest.cpp
    CommandTest.cpp
    CostTableTest.cpp
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

    Utils/STLUtilsTest.cpp
)

# target_compile_features(AWCTest PRIVATE cxx_std_20)
target_include_directories(AWCTest PUBLIC ${DOCTEST_INCLUDE_DIR} ../include include)
target_link_libraries(AWCTest AWCLibrary AWCSer)

add_compile_definitions(PROJECT_DIR="${CMAKE_SOURCE_DIR}")

add_test(NAME BasicTest COMMAND AWCTest)
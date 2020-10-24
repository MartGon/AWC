add_executable(AWCTest 
    AWCTest.cpp
    CommandTest.cpp
    CostTableTest.cpp
    MapTest.cpp
    TileTest.cpp
    UnitTest.cpp
    TilePatternTest.cpp
    TilePatternDescriptorTest.cpp
    TileGraphTest.cpp
    STLUtilsTest.cpp
)

# target_compile_features(AWCTest PRIVATE cxx_std_20)
target_include_directories(AWCTest PUBLIC ${DOCTEST_INCLUDE_DIR} ../include)
target_link_libraries(AWCTest AWCLibrary)

add_test(NAME BasicTest COMMAND AWCTest)
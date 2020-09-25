add_executable(AWCTest 
    AWCTest.cpp
    MapTest.cpp
    TileTest.cpp
    UnitTest.cpp
    CommandTest.cpp
)

target_compile_features(AWCTest PRIVATE cxx_std_17)
target_include_directories(AWCTest PUBLIC ${DOCTEST_INCLUDE_DIR} ../include)
target_link_libraries(AWCTest AWCLibrary)

add_test(NAME BasicTest COMMAND AWCTest)
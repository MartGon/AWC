SET(SOURCES

    src/DBTest.cpp

    src/AWCSerTest.cpp
    src/TilePatternDescSerTest.cpp
    src/JsonUtilsTest.cpp
)

add_executable(DBTest ${SOURCES})

target_include_directories(DBTest PUBLIC include
    ${CMAKE_SOURCE_DIR}/src/AWC/include
    ${CMAKE_SOURCE_DIR}/src/DB/include 
    ${CMAKE_SOURCE_DIR}/src/Utils/include 

    ../AWC/include

    ${LUA_INCLUDE_DIR}
    ${DOCTEST_INCLUDE_DIR}
    ${NLOHMANN_JSON_INCLUDE_DIR})

target_link_libraries(DBTest AWC DB Utils AWCTestLib ${LUA_LIB_PATH})

add_test(NAME RunDBTest COMMAND DBTest)
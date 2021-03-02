
SET(SOURCES

    src/ScriptTest.cpp

    src/UserData/Game.cpp
    src/UserData/Map.cpp
    src/UserData/UserData.cpp
)

add_executable(ScriptTest ${SOURCES})
target_include_directories(ScriptTest PUBLIC include
    ${CMAKE_SOURCE_DIR}/src/Utils/include
    ${CMAKE_SOURCE_DIR}/src/AWC/include
    ${CMAKE_SOUCE_DIR}/src/Script/include    
    ${DOCTEST_INCLUDE_DIR})
target_link_libraries(ScriptTest AWC Script Utils AWCTestLib)

add_test(NAME RunScriptTest COMMAND ScriptTest)
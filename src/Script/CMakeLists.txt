set(SOURCES 
    src/ScriptType.cpp
    src/ScriptOperation.cpp
    src/ScriptGame.cpp

    src/UserData/Game.cpp
    src/UserData/Map.cpp
    src/UserData/Unit.cpp
    src/UserData/UserData.cpp
    src/UserData/Vector2.cpp

)

add_library(Script ${SOURCES})

target_link_libraries(Script Lua)
message("Path to lua is ${LUA_INCLUDE_DIR}")
target_include_directories(Script PUBLIC  include ../Utils/include ../AWC/include ${LUA_INCLUDE_DIR})
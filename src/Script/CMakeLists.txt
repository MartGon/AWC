set(SOURCES 
    src/ScriptType.cpp
    src/ScriptOperation.cpp
    src/Game.cpp
    src/UserData.cpp

    src/DB/Database.cpp

    src/UserData/CostTable.cpp
    src/UserData/Game.cpp
    src/UserData/Map.cpp
    src/UserData/Unit.cpp
    src/UserData/UserData.cpp
    src/UserData/MoveArea.cpp
    src/UserData/Vector2.cpp
    src/UserData/AreaDesc.cpp
    src/UserData/MovementDescType.cpp
    src/UserData/Database.cpp
    src/UserData/Player.cpp
    src/UserData/UnitType.cpp
    src/UserData/TileType.cpp
    src/UserData/Tile.cpp
    src/UserData/Process.cpp
    src/UserData/Trigger.cpp
    src/UserData/WeaponType.cpp
    src/UserData/AttackArea.cpp
    src/UserData/EventHandler.cpp
    src/UserData/EventNotification.cpp
    src/UserData/EntityGUID.cpp
    src/UserData/Operation.cpp

    src/Utils/Utils.cpp

    src/Wrappers/LuaTable.cpp
    src/Wrappers/LuaVM.cpp
    src/Wrappers/LuaFunction.cpp
    src/Wrappers/LuaCallback.cpp
)

add_library(Script ${SOURCES})

target_link_libraries(Script Lua)
message("Path to lua is ${LUA_INCLUDE_DIR}")
target_include_directories(Script PUBLIC  include ../Utils/include ../AWC/include ${LUA_INCLUDE_DIR})
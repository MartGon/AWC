set(SOURCES 
    src/ScriptType.cpp
    src/ScriptOperation.cpp
    src/ScriptGame.cpp

)

add_library(Script ${SOURCES})

target_link_libraries(Script ${LUA_LIB_PATH})
target_include_directories(Script PUBLIC  include ../Utils/include ../AWC/include ${LUA_INCLUDE_DIR})
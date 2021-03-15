SET(SOURCES
    src/AWCSer.cpp
    src/JsonUtils.cpp
    src/Repository.cpp
    src/AreaDescSer.cpp
    src/TileSer.cpp
)

add_library(DB
    ${SOURCES}
)

target_include_directories(DB PUBLIC include ../AWC/include ../Utils/include ${NLOHMANN_JSON_INCLUDE_DIR})
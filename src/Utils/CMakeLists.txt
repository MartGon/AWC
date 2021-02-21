SET(SOURCES
    src/Vector2.cpp
    src/STLUtils.cpp
)

add_library(Utils ${SOURCES})
target_include_directories(Utils PUBLIC include)
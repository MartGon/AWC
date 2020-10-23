add_library(AWCUtils
    Vector2.cpp
    STLUtils.cpp
    )

target_include_directories(AWCUtils PUBLIC ${CMAKE_SOURCE_DIR}/include)
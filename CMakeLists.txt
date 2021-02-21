# Set minimum CMake version
cmake_minimum_required(VERSION 3.1...3.16)

# Set project name
project(AWC VERSION 1.0 LANGUAGES CXX)

# Set C++ version
set (CMAKE_CXX_STANDARD 17)

# Add compile defines
if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    add_compile_definitions(_DEBUG=1)
endif()

add_subdirectory(extern)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)

    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif()

endif()

message("Build type is: ${CMAKE_BUILD_TYPE}")
message("Lua lib path is: ${LUA_LIB_PATH}")
message("Lua inlcude dir is: ${LUA_INCLUDE_DIR}")

add_subdirectory(src)
add_subdirectory(app)

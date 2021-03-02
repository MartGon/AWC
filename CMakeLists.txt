# Set minimum CMake version
cmake_minimum_required(VERSION 3.1...3.16)

# Set project name
project(AWC VERSION 1.0 LANGUAGES CXX C)

# Set C++ version
set (CMAKE_CXX_STANDARD 17)

# Add compile defines
if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    add_compile_definitions(_DEBUG=1)
endif()

message("Build type is: ${CMAKE_BUILD_TYPE}")

add_subdirectory(deps)
add_subdirectory(src)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)

    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif()

endif()

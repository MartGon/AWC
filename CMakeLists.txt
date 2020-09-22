# Set minimum CMake version
cmake_minimum_required(VERSION 3.1...3.16)

# Set project name
project(AWC VERSION 1.0 LANGUAGES CXX)

add_subdirectory(extern)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)

    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif()

endif()

add_subdirectory(src)
add_subdirectory(app)
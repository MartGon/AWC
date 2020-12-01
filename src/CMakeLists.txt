
add_subdirectory(AWC)
add_subdirectory(Utils)
add_subdirectory(AWCSer)

add_library(AWCLibrary AWC.cpp)

target_include_directories(AWCLibrary PUBLIC ../include)
target_link_libraries(AWCLibrary AWCSource AWCUtils AWCSer)

source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
)
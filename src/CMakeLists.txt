
add_library(AWCLibrary 
  AWC.cpp
)

target_include_directories(AWCLibrary PUBLIC ../include)

source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
)
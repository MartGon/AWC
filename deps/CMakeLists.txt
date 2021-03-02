set(DOCTEST_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/doctest CACHE INTERNAL "Path to include folder for doctest")
set(NLOHMANN_JSON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/json CACHE INTERNAL "Path to include nlohmann json lib")

add_subdirectory(lua)
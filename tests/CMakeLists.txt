add_compile_definitions(PROJECT_DIR="${CMAKE_SOURCE_DIR}")
add_compile_definitions(RESOURCES_DIR="${CMAKE_SOURCE_DIR}/resources/test/AWCSer/")
add_compile_definitions(SCRIPTS_DIR="${CMAKE_SOURCE_DIR}/resources/tests/Scripts/")

add_subdirectory(AWC)
add_subdirectory(DB)
add_subdirectory(Extern)
add_subdirectory(Utils)

add_executable(FullTest FullTest.cpp)

target_include_directories(FullTest PUBLIC ${DOCTEST_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/include include ${LUA_INCLUDE_DIR})
target_link_libraries(FullTest AWC DB Utils ${LUA_LIB_PATH})

add_test(NAME BasicTest COMMAND FullTest)
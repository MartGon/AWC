add_compile_definitions(PROJECT_DIR="${CMAKE_SOURCE_DIR}")
add_compile_definitions(RESOURCES_DIR="${CMAKE_SOURCE_DIR}/resources/tests/AWCSer/")
add_compile_definitions(SCRIPTS_DIR="${CMAKE_SOURCE_DIR}/resources/tests/Scripts/")

add_subdirectory(AWC)
add_subdirectory(DB)
add_subdirectory(Utils)
add_subdirectory(Script)
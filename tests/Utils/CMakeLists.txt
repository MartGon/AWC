SET(SOURCES

    src/STLUtilsTest.cpp
)

add_executable(UtilsTest ${SOURCES})
target_include_directories(UtilsTest PUBLIC include
    ${CMAKE_SOURCE_DIR}/src/Utils/include  
    ${DOCTEST_INCLUDE_DIR})
target_link_libraries(UtilsTest Utils)

add_test(NAME RunUtilsTest COMMAND UtilsTest)
add_library(AWCSer
    AWCSer.cpp
    JsonUtils.cpp
    Repository.cpp
)

target_include_directories(AWCSer PUBLIC ${CMAKE_SOURCE_DIR}/include ${NLOHMANN_JSON_INCLUDE_DIR})
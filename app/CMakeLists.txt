add_executable(AWCExecutable 
    AWCMain.cpp    
)

target_include_directories(AWCExecutable PUBLIC ../include)
target_link_libraries(AWCExecutable AWCSource AWCUtils)
add_executable(AWCExecutable 

    AWCMain.cpp    
    Console.cpp
    ConsoleCommand.cpp
)

target_include_directories(AWCExecutable PUBLIC ../include include) 
target_link_libraries(AWCExecutable AWCSource AWCUtils)
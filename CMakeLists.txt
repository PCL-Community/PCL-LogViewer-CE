cmake_minimum_required(VERSION 3.10)

project(PCL2-LogViewer-CE VERSION 1.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

add_executable(TextViewer main.c)

if(WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    
    target_link_libraries(TextViewer comctl32)
endif()

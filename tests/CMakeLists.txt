cmake_minimum_required(VERSION 3.14)
project(AllTests)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/model
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/data-structure
    ${CMAKE_CURRENT_SOURCE_DIR}/../core
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/map-generation
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/view
    ${CMAKE_CURRENT_SOURCE_DIR} 
)

add_executable(run_all_tests 
    TestRunner.cpp 
    TestGraph.cpp 
    TestPq.cpp
)
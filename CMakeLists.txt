cmake_minimum_required(VERSION 3.10)
project(GraphAdjacencyList)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create a library target
add_library(graph_lib
    src/graph.cpp
    include/graph.h
    include/directed_graph.h
    include/dfs_graph_processing.h
)

# Make the include directory available
target_include_directories(graph_lib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# Main executable
add_executable(graph_main src/main.cpp)
target_link_libraries(graph_main PRIVATE graph_lib)

# Testing configuration
option(BUILD_TESTS "Build tests" ON)

if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.4.0
    )
    FetchContent_MakeAvailable(Catch2)

    add_executable(graph_tests tests/test_graph.cpp tests/test_directed_graph.cpp)
    target_link_libraries(graph_tests PRIVATE graph_lib Catch2::Catch2WithMain)
endif()

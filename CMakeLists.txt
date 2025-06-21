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
    include/directed_cycle.h
    include/topological_sort.h
    include/symbol_graph.h
    include/PGraph.h
    include/GraphVisualizer.h
    include/dfs_graph.h
    include/bfs_graph.h
    include/Path.h
    include/CC.h
    include/VGraph.h
)

# Make the include directory available
target_include_directories(graph_lib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Audio Network)

# Main executable
add_executable(graph_main src/main.cpp)
target_link_libraries(graph_main PRIVATE graph_lib SFML::Graphics SFML::Audio SFML::Network)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
endif()

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

    add_executable(graph_tests 
    tests/test_graph.cpp 
    tests/test_directed_graph.cpp 
    tests/test_cycle.cpp 
    tests/tests_topological_sort.cpp
    tests/tests_symbol_graph.cpp
    tests/tests_p_graph.cpp
    tests/tests_dfs.cpp
    )
    target_link_libraries(graph_tests PRIVATE graph_lib Catch2::Catch2WithMain)
endif()

cmake_minimum_required(VERSION 3.16)
project(FileTree LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create a library from your .cpp functions
add_library(filetree_lib
    src/args.cpp
    src/chars.cpp
    src/filetree.cpp
)

# Public header directory
target_include_directories(filetree_lib PUBLIC include)

# Main executable
add_executable(filetree src/main.cpp)
target_link_libraries(filetree PRIVATE filetree_lib)

# Enable tests
enable_testing()
add_subdirectory(tests)

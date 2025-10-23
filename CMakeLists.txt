cmake_minimum_required(VERSION 3.15)
project(tree_plus LANGUAGES CXX)

if(MINGW)
    message(STATUS "Building with mingw-w64")
endif()

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include headers
include_directories(${CMAKE_SOURCE_DIR}/include)

# Collect all source files
file(GLOB SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

if (MINGW)
    message(STATUS "Configuring for MinGW — enabling static linking")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
else()
    message(STATUS "Not using MinGW — skipping static linking flags")
endif()

# Optionally set output directory for binary
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

cmake_minimum_required(VERSION 3.11)
project(fps_game C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 99)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Add include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src/headers
    /usr/local/include
    /opt/homebrew/include
    /Users/skye/Downloads/raygui-4.0/src
)

# Find or fetch raylib
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} REQUIRED)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_MakeAvailable(raylib)
endif()

# Collect source files
file(GLOB_RECURSE SRC_FILES
    src/*.c
)

# Define executable
add_executable(game ${SRC_FILES})

# Link libraries
target_link_libraries(game raylib m dl pthread)

# macOS frameworks
if (APPLE)
    target_link_libraries(game "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
endif()

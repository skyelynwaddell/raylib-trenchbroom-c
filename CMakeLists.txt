cmake_minimum_required(VERSION 3.11)
project(fps_game C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Include project headers and raylib headers
include_directories(
    ${CMAKE_SOURCE_DIR}/src/headers
    ${CMAKE_SOURCE_DIR}/src/skyelib
    ${CMAKE_SOURCE_DIR}/src/lib/raylib/include
    /Users/skye/Downloads/raygui-4.0/src
    /opt/homebrew/include    # SDL2 headers (Homebrew default on Apple Silicon)
)

# SDL2: locate the SDL2 library installed via Homebrew
find_library(SDL2_LIBRARY SDL2 HINTS /opt/homebrew/lib /usr/local/lib)

if(NOT SDL2_LIBRARY)
    message(FATAL_ERROR "SDL2 library not found. Install it with: brew install sdl2")
endif()

# Manually import the precompiled raylib static library
add_library(raylib STATIC IMPORTED GLOBAL)
set_target_properties(raylib PROPERTIES
    IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/src/lib/raylib/lib/libraylib.a
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/src/lib/raylib/include
)

# Gather all C source files
file(GLOB_RECURSE SRC_FILES src/*.c)

# Define the executable
add_executable(game ${SRC_FILES})

# Link your executable with raylib, SDL2, and system libs
target_link_libraries(game
    raylib
    ${SDL2_LIBRARY}
    m
    dl
    pthread
)

# Link required macOS frameworks
if(APPLE)
    target_link_libraries(game
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()

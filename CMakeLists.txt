cmake_minimum_required(VERSION 3.11)
project(fps_game C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output binaries to /bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Include project headers
include_directories(
    ${CMAKE_SOURCE_DIR}/src/headers
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/skyelib
    ${CMAKE_SOURCE_DIR}/src/lib/raylib/include
    /Users/skye/Downloads/raygui-4.0/src
)

# === SDL2 (Homebrew on macOS) ===
find_path(SDL2_INCLUDE_DIR SDL.h HINTS /opt/homebrew/include/SDL2 /usr/local/include/SDL2)
find_library(SDL2_LIBRARY SDL2 HINTS /opt/homebrew/lib /usr/local/lib)

if(NOT SDL2_INCLUDE_DIR OR NOT SDL2_LIBRARY)
    message(FATAL_ERROR "SDL2 not found. Install it with: brew install sdl2")
endif()

# Add SDL2 includes
include_directories(${SDL2_INCLUDE_DIR})

# === Raylib static library ===
add_library(raylib STATIC IMPORTED GLOBAL)
set_target_properties(raylib PROPERTIES
    IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/src/lib/raylib/lib/libraylib.a
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/src/lib/raylib/include
)

# === Source files ===
file(GLOB_RECURSE SRC_FILES src/*.c)
list(APPEND SRC_FILES src/lib/raylib/include/raygui.h)

# === Executable ===
add_executable(game ${SRC_FILES})

# === Link libraries ===
target_link_libraries(game
    raylib
    ${SDL2_LIBRARY}
    m
    dl
    pthread
)

# === macOS Frameworks ===
if(APPLE)
    target_link_libraries(game
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()

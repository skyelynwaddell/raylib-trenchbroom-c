cmake_minimum_required(VERSION 3.11)
project(fps_game C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src/headers
    ${CMAKE_SOURCE_DIR}/src/skyelib
    ${CMAKE_SOURCE_DIR}/src/lib/raylib/include
    /Users/skye/Downloads/raygui-4.0/src # Optional: platform-specific
)

# Platform-specific SDL2 setup
if(APPLE)
    find_library(SDL2_LIBRARY SDL2 HINTS /opt/homebrew/lib /usr/local/lib)
    include_directories(/opt/homebrew/include)
elseif(UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    include_directories(${SDL2_INCLUDE_DIRS})
endif()

if(NOT SDL2_LIBRARY AND NOT SDL2_LIBRARIES)
    message(FATAL_ERROR "SDL2 library not found. On Linux, try: sudo apt install libsdl2-dev")
endif()

# Detect platform and set Raylib lib folder accordingly
if(APPLE)
    set(RAYLIB_LIB_DIR ${CMAKE_SOURCE_DIR}/src/lib/raylib/lib/mac)
elseif(UNIX)
    set(RAYLIB_LIB_DIR ${CMAKE_SOURCE_DIR}/src/lib/raylib/lib/linux)
else()
    message(FATAL_ERROR "Unsupported platform for raylib static library")
endif()

# Raylib static library (precompiled)
add_library(raylib STATIC IMPORTED GLOBAL)
set_target_properties(raylib PROPERTIES
    IMPORTED_LOCATION ${RAYLIB_LIB_DIR}/libraylib.a
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/src/lib/raylib/include
)

# Gather source files
file(GLOB_RECURSE SRC_FILES src/*.c)

# Define executable
add_executable(game ${SRC_FILES})

# Link libraries
if(APPLE)
    target_link_libraries(game
        raylib
        ${SDL2_LIBRARY}
        m dl pthread
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
elseif(UNIX)
    target_link_libraries(game
        raylib
        ${SDL2_LIBRARIES}
        m dl pthread GL X11
    )
endif()
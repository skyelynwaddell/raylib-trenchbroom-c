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

# Platform-specific SDL2 and ENet setup
if(APPLE)
    find_library(SDL2_LIBRARY SDL2 HINTS /opt/homebrew/lib /usr/local/lib)
    include_directories(/opt/homebrew/include)

    # Try to find ENet manually on macOS (adjust if needed)
    find_library(ENET_LIBRARY enet HINTS /usr/local/lib /opt/homebrew/lib)
    if(NOT ENET_LIBRARY)
        message(FATAL_ERROR "ENet library not found on macOS. Please install it.")
    endif()
    include_directories(/usr/local/include /opt/homebrew/include)

elseif(UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    include_directories(${SDL2_INCLUDE_DIRS})

    # Try pkg-config for ENet, fallback if not found
    execute_process(
        COMMAND pkg-config --exists enet
        RESULT_VARIABLE ENET_PKG_OK
    )
    if(ENET_PKG_OK EQUAL 0)
        message(STATUS "Found ENet via pkg-config")
        pkg_check_modules(ENET REQUIRED enet)
        include_directories(${ENET_INCLUDE_DIRS})
        link_directories(${ENET_LIBRARY_DIRS})
        set(ENET_LIBS ${ENET_LIBRARIES})
    else()
        message(WARNING "ENet not found via pkg-config; using manual paths")
        include_directories(/usr/include)
        link_directories(/usr/lib/x86_64-linux-gnu)
        set(ENET_LIBS enet)
    endif()
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

file(GLOB_RECURSE SRC_FILES src/*.c) # Game Source Files
file(GLOB SERVER_SRC_FILES server/*.c) # Server Source Files
list(FILTER SRC_FILES EXCLUDE REGEX ".*src/main.c") # Exclude main.c from game source files

add_executable(game src/main.c ${SRC_FILES}) # Include main.c in the game executable
add_executable(server ${SRC_FILES} ${SERVER_SRC_FILES})

# Link libraries
if(APPLE)
    target_link_libraries(game
        raylib
        ${SDL2_LIBRARY}
        ${ENET_LIBRARY}
        m dl pthread
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )

    target_link_libraries(server
        raylib
        ${SDL2_LIBRARY}
        ${ENET_LIBRARY}
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
        ${ENET_LIBS}
        m dl pthread GL X11
    )

    target_link_libraries(server
        raylib
        ${SDL2_LIBRARIES}
        ${ENET_LIBS}
        m dl pthread GL X11
    )
endif()

cmake_minimum_required(VERSION 3.20)

project(geometric-mechanics-sandbox VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    versor
    GIT_REPOSITORY https://github.com/wolftype/versor.git
    GIT_TAG 8262fce98c99ea6d9f0984d2ac17cde9fe5d0519
    GIT_SHALLOW TRUE
)
FetchContent_GetProperties(versor)
if(NOT versor_POPULATED)
    FetchContent_Populate(versor)
endif()

# Prefer a known-good system package when available. Otherwise CMake obtains
# raylib automatically, keeping the project self-contained on other systems.
find_package(raylib 5.5 QUIET)

if(NOT TARGET raylib)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(raylib)
endif()

set(SOURCES
    src/main.cpp
    src/Application.cpp
    src/Simulation.cpp
    src/Renderer.cpp
)

add_executable(geometric-mechanics-sandbox ${SOURCES})

if(APPLE)
    set_target_properties(geometric-mechanics-sandbox PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER com.example.geometricmechanicssandbox
        MACOSX_BUNDLE_BUNDLE_NAME "Geometric Mechanics Sandbox"
        BUILD_RPATH "/opt/homebrew/opt/raylib/lib"
        INSTALL_RPATH "/opt/homebrew/opt/raylib/lib"
    )
endif()

target_include_directories(geometric-mechanics-sandbox PRIVATE src)
target_include_directories(geometric-mechanics-sandbox PRIVATE
    ${versor_SOURCE_DIR}/include
)
set_property(TARGET geometric-mechanics-sandbox PROPERTY CXX_STANDARD 20)
target_link_libraries(geometric-mechanics-sandbox PRIVATE raylib)

if(APPLE)
    find_library(COCOA_FRAMEWORK Cocoa REQUIRED)
    find_library(IOKIT_FRAMEWORK IOKit REQUIRED)
    find_library(OPENGL_FRAMEWORK OpenGL REQUIRED)
    target_link_libraries(geometric-mechanics-sandbox PRIVATE
        ${COCOA_FRAMEWORK}
        ${IOKIT_FRAMEWORK}
        ${OPENGL_FRAMEWORK}
    )
endif()

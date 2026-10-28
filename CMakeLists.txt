cmake_minimum_required(VERSION 3.20)

project(geometric-mechanics-sandbox VERSION 0.1.0 LANGUAGES CXX)

include(CTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG 3.4.0
    GIT_SHALLOW TRUE
)
set(EIGEN_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(project_build_testing ${BUILD_TESTING})
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(eigen)
set(BUILD_TESTING ${project_build_testing} CACHE BOOL "" FORCE)
if(NOT TARGET Eigen3::Eigen)
    add_library(Eigen3::Eigen ALIAS eigen)
endif()
set(GEOMETRIC_MECHANICS_EIGEN_SOURCE_DIR "${eigen_SOURCE_DIR}")
set(Eigen3_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

FetchContent_Declare(
    gafro
    GIT_REPOSITORY https://gitlab.com/gafro/gafro.git
    GIT_TAG 5a8a5ec2cfd0d80b74017565f50824b9571a7e69
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(gafro)
# The upstream target exports Eigen's legacy include path as an install
# interface. That is valid after installation but invalid for a fetched source
# tree, so retain only the two build interfaces this application needs.
set_property(TARGET gafro PROPERTY INTERFACE_INCLUDE_DIRECTORIES
    "$<BUILD_INTERFACE:${gafro_SOURCE_DIR}/src>;$<BUILD_INTERFACE:${eigen_SOURCE_DIR}>"
)

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
set_property(TARGET geometric-mechanics-sandbox PROPERTY CXX_STANDARD 20)
target_link_libraries(geometric-mechanics-sandbox PRIVATE raylib gafro::gafro)

if(BUILD_TESTING)
    add_executable(planar-motor-tests tests/PlanarMotorTests.cpp)
    target_include_directories(planar-motor-tests PRIVATE src)
    target_link_libraries(planar-motor-tests PRIVATE raylib gafro::gafro)
    add_test(NAME planar-motor-tests COMMAND planar-motor-tests)
endif()

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

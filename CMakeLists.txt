cmake_minimum_required(VERSION 3.16)

project(Commanche2D)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wfatal-errors -g")
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_OBJC_FLAGS "${CMAKE_OBJC_FLAGS} -x objective-c")

# Compiler flags
add_compile_options(-Wall -Wfatal-errors -g)
add_definitions(-DPLATFORM_DESKTOP)

# Sub dirs
add_subdirectory(libs/raylib)

# Find required packages using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(FREETYPE2 REQUIRED freetype2)

# Find ANGLE libraries
set(ANGLE_LIBRARY_DIR libs/angle/out/Release)
find_library(ANGLE_GLESv2_LIBRARY libGLESv2.dylib PATHS ${ANGLE_LIBRARY_DIR})
find_library(ANGLE_EGL_LIBRARY libEGL.dylib PATHS ${ANGLE_LIBRARY_DIR})

# Include directories
include_directories(
    "./libs"
    "./libs/lua"
    "./libs/angle/include"
    "./libs/raylib/src"
    "./libs/rlImGui"
    "./libs/imgui"
    "./libs/box2d/include"
    "./libs/imgui/backends"
    "/opt/homebrew/include"
    "./libs/glad/include"
    "./libs/stb"
    "./libs/flecs/include"
    ${GLFW3_INCLUDE_DIRS}
    ${FREETYPE2_INCLUDE_DIRS}
)

file(GLOB FLECS_SOURCES libs/flecs/*.c libs/flecs/*.cpp)
add_library(flecs STATIC ${FLECS_SOURCES})

# Source files
file(GLOB SOURCES
    "./src/*.cpp"
    "./src/game/*.cpp"
    "./src/io/*.cpp"
    "./src/ecs/*.cpp"
    "./src/log/*.cpp"
    "./src/core/*.cpp"
    "./src/render/*.cpp"
    "./src/render/backends/render_raylib.cpp"
    "./src/physics/*.cpp"
    "./src/systems/*.cpp"
    "./libs/rlImGui/*.cpp"
    "./src/editor/*.cpp"
    "./src/editor/systems/*.cpp"
    "./src/scene/*.cpp"
    "./src/common/*.cpp"
    "./src/assetmgr/*.cpp"
    "./src/assetmgr/backends/asset_manager_raylib.cpp"
    "./src/scripting/*.cpp"
    "./libs/imgui/*.cpp"
    "./libs/box2d/src/*/*.cpp"
)


# Linker flags and libraries
link_directories(
  /opt/homebrew/lib
  "./libs"
)

# Build targets
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} raylib ${ANGLE_EGL_LIBRARY} ${ANGLE_GLESv2_LIBRARY} lua "-framework IOKit" "-framework Cocoa" flecs)

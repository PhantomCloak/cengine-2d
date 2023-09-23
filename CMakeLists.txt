cmake_minimum_required(VERSION 3.12)

project(Commanche2D)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wfatal-errors -g")
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Compiler flags
add_compile_options(-Wall -Wfatal-errors -g)

# Find required packages using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW3 REQUIRED glfw3)
pkg_check_modules(FREETYPE2 REQUIRED freetype2)


# Include directories
include_directories(
    "./libs"
    "./libs/lua"
    "./libs/imgui"
    "./libs/SimpleJSON"
    "./libs/box2d/include"
    "./libs/imgui/backends"
    "/opt/homebrew/include"
    "./libs/glad/include"
    "./libs/stb"
    "./libs/flecs/include"
    ${GLFW3_INCLUDE_DIRS}
    ${FREETYPE2_INCLUDE_DIRS}
)

file(GLOB FLECS_SOURCES libs/flecs/*.c libs/flecs/*.cpp)  # Adjust this if flecs source files are in different locations
add_library(flecs STATIC ${FLECS_SOURCES})

# Source files
file(GLOB SOURCES
    "./libs/glad/src/glad.c"
    "./src/*.cpp"
    "./src/game/*.cpp"
    "./src/io/*.cpp"
    "./src/ecs/*.cpp"
    "./src/log/*.cpp"
    "./src/core/*.cpp"
    "./src/physics/*.cpp"
    "./src/systems/*.cpp"
    "./src/editor/*.cpp"
    "./src/scene/*.cpp"
    "./src/common/*.cpp"
    "./src/assetmgr/*.cpp"
    "./src/scripting/*.cpp"
    "./src/render/*.cpp"
    "./src/render/backends/render_opengl.cpp"
    "./src/render/opengl/*.cpp"
    "./libs/imgui/*.cpp"
    "./libs/box2d/src/*/*.cpp"
    "./libs/imgui/backends/imgui_impl_glfw.cpp"
    "./libs/imgui/backends/imgui_impl_opengl3.cpp"
)

# Linker flags and libraries
link_directories(/opt/homebrew/lib)
link_libraries(
    ${GLFW3_LIBRARIES}
    ${FREETYPE2_LIBRARIES}
    lua
    flecs
    "-framework IOKit"
    "-framework Cocoa"
    "-framework OpenGL"
)

# Build targets
add_executable(commanche2d ${SOURCES})

add_executable(commanche2d_dev ${SOURCES})
target_compile_definitions(commanche2d_dev PRIVATE EDITOR=1)

# Custom target for running the executable
add_custom_target(run
    COMMAND ./commanche2d
    DEPENDS commanche2d
    USES_TERMINAL
)

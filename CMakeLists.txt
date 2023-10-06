cmake_minimum_required(VERSION 3.16)

project(Commanche2D)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wfatal-errors -g")
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_OBJC_FLAGS "${CMAKE_OBJC_FLAGS} -x objective-c")

set(CMAKE_CXX_FLAGS_DEBUG "-g")

# Compiler flags
add_compile_options(-Wall -Wfatal-errors -g)
add_definitions(-DPLATFORM_DESKTOP)

find_package(PkgConfig REQUIRED)

if(RENDER_BACKEND_RAYLIB)
    add_definitions(-DRENDER_BACKEND_RAYLIB=1)

    # Sub dirs
    add_subdirectory(libs/raylib)
    
    # Find ANGLE libraries
    set(ANGLE_LIBRARY_DIR libs/angle/out/Release)
    find_library(ANGLE_GLESv2_LIBRARY libGLESv2.dylib PATHS ${ANGLE_LIBRARY_DIR})
    find_library(ANGLE_EGL_LIBRARY libEGL.dylib PATHS ${ANGLE_LIBRARY_DIR})

    file(GLOB_RECURSE RAYLIB_SOURCES 
            "./src/io/backends/raylib/*.cpp"
            "./src/render/backends/raylib/*.cpp"
            "./src/assetmgr/backends/raylib/*.cpp"
            "./src/assetmgr/backends/asset_manager_raylib.cpp")

    file(GLOB IMGUI_SOURCES
        "./libs/rlImGui/*.cpp")

    list(APPEND SOURCES ${RAYLIB_SOURCES} ${IMGUI_SOURCES})

    include_directories(
        "./libs/raylib/src"
        "./libs/rlImGui")
endif()

if(RENDER_BACKEND_OPENGL)
        add_definitions(-DRENDER_BACKEND_OPENGL=1)
        pkg_check_modules(GLFW3 REQUIRED glfw3)
        pkg_check_modules(FREETYPE2 REQUIRED freetype2)

        file(GLOB_RECURSE OPENGL_SOURCES 
                "./libs/glad/src/glad.c"
                "./src/io/backends/opengl/*.cpp"
                "./src/render/backends/opengl/*.cpp"
                "./src/assetmgr/backends/opengl/*.cpp"
                "./src/assetmgr/backends/opengl/asset_manager_opengl.cpp")

        list(APPEND SOURCES ${OPENGL_SOURCES})

        include_directories(
                ${FREETYPE2_INCLUDE_DIRS})
endif()

if(ENGINE_EDITOR)
    add_definitions(-DEDITOR=1)
    file(GLOB EDITOR_SOURCES
        "./src/editor/*.cpp"
        "./src/editor/systems/*.cpp"
    )

if(RENDER_BACKEND_OPENGL)
        file(GLOB EDITOR_DEPS
        "./libs/imgui/backends/imgui_impl_glfw.cpp"
        "./libs/imgui/backends/imgui_impl_opengl3.cpp")

        list(APPEND EDITOR_SOURCES ${EDITOR_DEPS})
endif()

    list(APPEND SOURCES ${EDITOR_SOURCES})
    message(STATUS "Compiling with Editor")
endif()


# Include directories
include_directories(
    "./libs"
    "./libs/lua"
    "./libs/angle/include"
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
file(GLOB ENGINE_SOURCES
    "./src/*.cpp"
    "./src/game/*.cpp"
    "./src/io/*.cpp"
    "./src/ecs/*.cpp"
    "./src/log/*.cpp"
    "./src/core/*.cpp"
    "./src/render/*.cpp"
    "./src/physics/*.cpp"
    "./src/systems/*.cpp"
    "./src/scene/*.cpp"
    "./src/common/*.cpp"
    "./src/assetmgr/*.cpp"
    "./src/scripting/*.cpp"
    "./libs/imgui/*.cpp"
    "./libs/box2d/src/*/*.cpp"
)

list(APPEND SOURCES ${ENGINE_SOURCES})

# Linker flags and libraries
link_directories(
  /opt/homebrew/lib
  "./libs"
)

# Build targets
add_executable(${PROJECT_NAME} ${SOURCES})

#target_compile_definitions(${PROJECT_NAME} PRIVATE EDITOR=1)

target_link_libraries(${PROJECT_NAME} raylib ${ANGLE_EGL_LIBRARY} ${ANGLE_GLESv2_LIBRARY} lua "-framework IOKit" "-framework Cocoa" flecs ${FREETYPE2_LIBRARIES} ${GLFW3_LIBRARIES})

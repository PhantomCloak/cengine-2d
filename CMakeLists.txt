cmake_minimum_required(VERSION 3.10)

project(Commanche2D)
# Options
option(RENDER_BACKEND_RAYLIB "Use the Raylib backend" OFF)
option(RENDER_BACKEND_OPENGL "Use the OpenGL backend" OFF)
option(ENGINE_EDITOR "Enable the engine editor" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wfatal-errors -g")
    set(CMAKE_OBJC_FLAGS "${CMAKE_OBJC_FLAGS} -x objective-c")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable")
endif()
# Additional definitions
add_definitions(-DPLATFORM_DESKTOP)

# Find glm package using vcpkg
find_package(glm CONFIG REQUIRED)
find_package(lua CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(sol2 CONFIG REQUIRED)

# Include directories
include_directories(
    "./libs"
    "./libs/lua"
    "./libs/angle/include"
    "./libs/imgui"
    "./libs/box2d/include"
    "./libs/imgui/backends"
    "./libs/SimpleJSON"
    "/opt/homebrew/include"
    "./libs/glad/include"
    "./libs/stb"
    "./libs/flecs/include"
    "./libs/nativefiledialog-extended/src/include"
    "./libs/native_message_box/include"
)

# Linker directories
link_directories(
  /opt/homebrew/lib
  "./libs"
)

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
    "./libs/flecs/*.c"
    "./src/assetmgr/*.cpp"
    "./src/scripting/*.cpp"
    "./libs/SimpleJSON/*.cpp"
    "./libs/imgui/*.cpp"
    "./libs/box2d/src/*/*.cpp"
)

set(SOURCES ${ENGINE_SOURCES})

# Subdirectories
add_subdirectory(libs/nativefiledialog-extended)
add_subdirectory(libs/native_message_box)
add_subdirectory(libs/flecs)

if(RENDER_BACKEND_RAYLIB)
    add_definitions(-DRENDER_BACKEND_RAYLIB=1)
    add_subdirectory(libs/raylib)

    set(ANGLE_LIBRARY_DIR libs/angle/out/Release)
    find_library(ANGLE_GLESv2_LIBRARY libGLESv2.dylib PATHS ${ANGLE_LIBRARY_DIR})
    find_library(ANGLE_EGL_LIBRARY libEGL.dylib PATHS ${ANGLE_LIBRARY_DIR})

    file(GLOB RAYLIB_SOURCES 
            "./src/io/backends/raylib/*.cpp"
            "./src/render/backends/raylib/*.cpp"
            "./src/assetmgr/backends/raylib/*.cpp")
    
    file(GLOB IMGUI_SOURCES "./libs/rlImGui/*.cpp")
    
    include_directories("./libs/rlImGui")
    list(APPEND SOURCES ${RAYLIB_SOURCES} ${IMGUI_SOURCES})
endif()

if(RENDER_BACKEND_OPENGL)
    add_definitions(-DRENDER_BACKEND_OPENGL=1)
    
    if(WIN32)
        # Use vcpkg to handle GLFW3 and Freetype on Windows
        find_package(glfw3 CONFIG REQUIRED)
        find_package(freetype CONFIG REQUIRED)
    else()
        # Use pkg-config on non-Windows platforms
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(GLFW3 REQUIRED glfw3)
        pkg_check_modules(FREETYPE2 REQUIRED freetype2)
    endif()
endif()

if(ENGINE_EDITOR)
    add_definitions(-DEDITOR=1)
    file(GLOB EDITOR_SOURCES
        "./src/editor/*.cpp"
        "./src/editor/log_sinks/*.cpp"
        "./src/editor/systems/*.cpp")

    if(RENDER_BACKEND_OPENGL)
        file(GLOB EDITOR_DEPS
        "./libs/imgui/backends/imgui_impl_glfw.cpp"
        "./libs/imgui/backends/imgui_impl_opengl3.cpp")

        list(APPEND EDITOR_SOURCES ${EDITOR_DEPS})
    endif()

    list(APPEND SOURCES ${EDITOR_SOURCES})
endif()

# Build targets
add_executable(${PROJECT_NAME} ${SOURCES})

if(WIN32)
    target_include_directories(${PROJECT_NAME} PRIVATE ${LUA_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE nfd raylib glm::glm lua glfw sol2 ${LUA_LIBRARIES})
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE nfd raylib ${ANGLE_EGL_LIBRARY} ${ANGLE_GLESv2_LIBRARY} lua "-framework IOKit" "-framework Appkit" "-framework UniformTypeIdentifiers" "-framework Cocoa" ${FREETYPE2_LIBRARIES} ${GLFW3_LIBRARIES})
endif()

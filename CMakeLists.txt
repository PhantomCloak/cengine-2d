cmake_minimum_required(VERSION 3.10)

project(Commanche2D)

option(RENDER_BACKEND_RAYLIB "Use the Raylib backend" OFF)
option(RENDER_BACKEND_OPENGL "Use the OpenGL backend" OFF)
option(ENGINE_EDITOR "Compile with editor" OFF)
option(RAYLIB_ANGLE_SUPPORT "Enable ANGLE support in Raylib backend" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wfatal-errors -g")
    set(CMAKE_OBJC_FLAGS "${CMAKE_OBJC_FLAGS} -x objective-c")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable")
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wfatal-errors -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable")
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_OBJC_FLAGS "${CMAKE_OBJC_FLAGS} -x objective-c")
    endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_definitions(/W4)
    add_definitions(/WX)
    add_definitions(/EHsc)
endif()

add_definitions(-DPLATFORM_DESKTOP)

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

    if(RAYLIB_ANGLE_SUPPORT)
        set(ANGLE_LIBRARY_DIR libs/angle/out/Release)

        find_library(ANGLE_GLESv2_LIBRARY NAMES GLESv2 PATHS ${ANGLE_LIBRARY_DIR})
        find_library(ANGLE_EGL_LIBRARY NAMES EGL PATHS ${ANGLE_LIBRARY_DIR})
        add_definitions(-DCUSTOMIZE_BUILD=1 -DOPENGL_VERSION="ES 2.0")
    endif()

    add_subdirectory(libs/raylib)

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
    
    find_package(glfw3 CONFIG REQUIRED)
    find_package(freetype CONFIG REQUIRED)
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

target_include_directories(${PROJECT_NAME} PRIVATE ${LUA_INCLUDE_DIR})

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE nfd raylib glm::glm lua glfw sol2 ${LUA_LIBRARIES})
elseif(UNIX)
    if(RAYLIB_ANGLE_SUPPORT)
        target_link_libraries(${PROJECT_NAME} PRIVATE nfd raylib ${ANGLE_EGL_LIBRARY} ${ANGLE_GLESv2_LIBRARY} lua sol2 "-framework IOKit" "-framework Appkit" "-framework UniformTypeIdentifiers" "-framework Cocoa" ${FREETYPE2_LIBRARIES} ${GLFW3_LIBRARIES} ${LUA_LIBRARIES})
    else()
        target_link_libraries(${PROJECT_NAME} PRIVATE nfd raylib lua sol2 "-framework IOKit" "-framework Appkit" "-framework UniformTypeIdentifiers" "-framework Cocoa" ${FREETYPE2_LIBRARIES} ${GLFW3_LIBRARIES} ${LUA_LIBRARIES})
    endif()
endif()

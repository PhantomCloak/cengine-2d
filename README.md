# cengine-2d

cengine-2d is a cross-platform OpenGL 2D game engine.

[Watch the progress here](https://www.youtube.com/@ph4nt0m33)

<img width="3120" alt="image" src="https://github.com/PhantomCloak/cengine-2d/assets/34552014/df993eae-550e-46cd-bef0-a2e40ad93036">
<img width="3120" alt="image" src="https://github.com/PhantomCloak/cengine-2d/assets/34552014/cfa97fb9-b975-4b45-8546-848ff8799d59">
<img width="3120" alt="image" src="https://github.com/PhantomCloak/cengine-2d/assets/34552014/f5cbecf4-cb2e-4c1b-86f2-cd5ca591eacc">

### Compiler Flags

| FLAG | DESC  
| :---:   | :---: |
| -DENGINE_EDITOR | Compile with editor & tools   | 
| --DRENDER_BACKEND_RAYLIB=1 | Set Rendering Backend to raylib  | 
| --DRENDER_BACKEND_OPENGL=1 | Set Rendering Backend to OpenGL  |
| --DRAYLIB_ANGLE_SUPPORT=1 | Enable [Angle](https://github.com/google/angle) Support for raylib |

## Installation


### UNIX
```sh
 (mkdir build && cd build)
 cmake ..
 make
```

### Windows (Visual Studio)
```
Visual Studio -> Open -> CMake -> CMakeLists.txt
Visual Studio -> Project -> CMake Settings for Commanche2D -> CMake Variables -> Pick OpenGL or Raylib backend
```

### VCPKG Dependencie
```
vcpkg install lua
vcpkg install sol2
vcpkg install glm
vcpkg install glfw3 -- if OpenGL
vcpkg install freetype - if OpenGL
```
## Features

### Scripting
Uses LUA for game mechanics and interactions.

### Entity Component System
Built on the FLECS library for managing game entities and components.

### Physics Engine
Implements Box2D for game object physics and collisions.

### Render & Shading
- Raylib for rendering, with ANGLE support for platform compatibility.
- A custom OpenGL renderer for more specific rendering needs.

#### Animations (WIP)
Native animation designer

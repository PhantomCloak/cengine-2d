# cengine-2d

cengine-2d is a cross-platform OpenGL 2D game engine.

[Watch the progress here](https://www.youtube.com/@ph4nt0m33)

<img width="3120" alt="image" src="https://github.com/PhantomCloak/cengine-2d/assets/34552014/df993eae-550e-46cd-bef0-a2e40ad93036">
<img width="3120" alt="image" src="https://github.com/PhantomCloak/cengine-2d/assets/34552014/cfa97fb9-b975-4b45-8546-848ff8799d59">
<img width="3120" alt="image" src="https://github.com/PhantomCloak/cengine-2d/assets/34552014/f5cbecf4-cb2e-4c1b-86f2-cd5ca591eacc">
<img width="3120" alt="image" src="https://github.com/PhantomCloak/cengine-2d/assets/34552014/4d61800f-26d4-4c6a-bdd4-69d663ec9245">

### Compiler Flags

| FLAG | DESC  
| :---:   | :---: |
| -DENGINE_EDITOR | Compile with editor & tools   | 
| --DRENDER_BACKEND_RAYLIB=1 | Set Rendering Backend to raylib  | 
| --DRENDER_BACKEND_OPENGL=1 | Set Rendering Backend to OpenGL  |
| --DRAYLIB_ANGLE_SUPPORT=1 | Enable [Angle](https://github.com/google/angle) Support for raylib |

## Installation


### UNIX (CMake & Make)
```sh
 (mkdir build && cd build)
 cmake -DENGINE_EDITOR=1 -DRENDER_BACKEND_OPENGL=1 -DCMAKE_TOOLCHAIN_FILE=[vcpkg path]/scripts/buildsystems/vcpkg.cmake  ..      
 make #-j N for parallel
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
LUA with C++ bindings.

### Entity Component System
Built on the FLECS library.

### Serialization
Default serialization format used in world/entities/physics is JSON.

### Physics Engine
Implements Box2D also supports custom-made physics engines.

### Render & Shading
Engine supports two different rendering backend.
- Raylib with out of box [ANGLE](https://github.com/google/angle) support for platform compatibility such as iOS.
- Modern OpenGL

### Export
Engine can built itself to following platforms.
 - MacOS
 - Linux
 - Windows

### Animations (WIP)
Native animation designer with timeline.

# cengine-2d

cengine-2d is a hobby game engine project, designed with flexibility and simplicity in mind.

[Watch the progress here](https://www.youtube.com/@ph4nt0m33)

<img width="3120" alt="image" src="https://github.com/PhantomCloak/cengine-2d/assets/34552014/df993eae-550e-46cd-bef0-a2e40ad93036">
<img width="3120" alt="image" src="https://github.com/PhantomCloak/cengine-2d/assets/34552014/240c102e-9437-443b-8b44-8802dadf94ce">

## Setup

```sh
 (mkdir build && cd build)
 cmake ..
 make
```

### Compiler Flags

| FLAG | DESC  
| :---:   | :---: |
| -DENGINE_EDITOR | Compile with editor & tools   | 
| --DRENDER_BACKEND_RAYLIB=1 | Set rendering backend to raylib  | 
| --DRENDER_BACKEND_OPENGL=1 | Set rendering backend to OpenGL  |
| --DCUSTOMIZE_BUILD=1 -DOPENGL_VERSION="ES 2.0" | hint raylib to use (Angle)[https://github.com/google/angle] |

### Dependencies MacOS

```
brew install glfw
brew install lua
brew install luarocks
luarocks install dkjson
brew install pkg-config
brew install glm
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

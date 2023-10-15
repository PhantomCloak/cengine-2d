#pragma once

struct CommancheColorRGB {
    int r;
    int g;
    int b;
};

struct CommancheColorRGBA {
    int r;
    int g;
    int b;
    int a;
};

struct CommancheTextureInfo {
    int height;
    int width;
};

struct CommancheRect {
    float x;
    float y;
    float height;
    float width;

    CommancheRect(float x = 0, float y = 0, float width = 0, float height = 0) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }
};

struct glCamera {
    float x;
    float y;
    float height;
    float width;
    int gridSize;

    glCamera(float x = 0, float y = 0, float width = 0, float height = 0, float gridSize = 0) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->gridSize = 40;
    }
};

struct glRectTangle {
    float x;
    float y;
    float height;
    float width;
};

#if RENDER_BACKEND_RAYLIB
#include "raylib.h"
#if defined(_WIN32)           
	#define NOGDI             // All GDI defines and routines
	#define NOUSER            // All USER defines and routines
#endif


#if defined(_WIN32)           // raylib uses these names as function parameters
  #include <Windows.h> // or any library that uses Windows.h
	#undef near
	#undef far
#endif

typedef Camera2D CommancheCamera;
typedef RenderTexture CommancheRenderTexture;
typedef Texture2D CommancheTexture;
#endif

#if RENDER_BACKEND_OPENGL
typedef glCamera CommancheCamera;
typedef unsigned int CommancheRenderTexture;
#endif

static float glBoxVertices[] = {
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // Lower left corner
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  // Upper left corner
    0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // Upper right corner
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f   // Lower right corner
};

static float glBoxIndices[] = {
    0, 2, 1, // Upper triangle
    0, 3, 2  // Lower triangle
};

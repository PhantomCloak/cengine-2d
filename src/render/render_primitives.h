#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

struct ViewRect {
    float left;
    float right;
    float top;
    float bottom;
};

struct glCamera {
    ViewRect target;
    glm::mat4 ProjectionMat;

    void SetTarget(const ViewRect& newTarget) {
        target = newTarget;
        UpdateProjection();
    }

    void SetCameraZoom(float zoom) {
        float width = target.right - target.left;
        float height = target.top - target.bottom;

        float centerX = (target.left + target.right) / 2.0f;
        float centerY = (target.top + target.bottom) / 2.0f;

        float halfWidth = (width * zoom) / 2.0f;
        float halfHeight = (height * zoom) / 2.0f;

        target.left = centerX - halfWidth;
        target.right = centerX + halfWidth;
        target.top = centerY + halfHeight;
        target.bottom = centerY - halfHeight;

        UpdateProjection();
    }

    void OffsetCamera(float vertical, float horizontal) {
        target.left += horizontal;
        target.right += horizontal;
        target.top += vertical;
        target.bottom += vertical;

        UpdateProjection();
    }

    void UpdateProjection() {
        ProjectionMat = glm::ortho(target.left, target.right, target.top, target.bottom, -1.0f, 1.0f);
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
#define NOGDI  // All GDI defines and routines
#define NOUSER // All USER defines and routines
#endif


#if defined(_WIN32)  // raylib uses these names as function parameters
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

enum class VBOType {
    POSITION,
    UV,
    NORMAL,
    COLOR,
    INTERLEAVED
};

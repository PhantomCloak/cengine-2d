#pragma once

#include <glad/glad.h>
#include <vector>
#include <map>
#define RECT_PRIMITIVE 0
#define LINE_PRIMITIVE 1

#define DEFAULT_SHADER_SLOT 0
#define DEFAULT_FONT_SHADER_SLOT 1
#define SOLID_RENDER_SHADER 2

static float glRectVertices[] = {
    -0.5f, -0.5f, 0.0f, // Lower left corner
    -0.5f, 0.5f,  0.0f, // Upper left corner
    0.5f,  0.5f,  0.0f, // Upper right corner
    0.5f,  -0.5f, 0.0f,  // Lower right corner
};

enum class VertexLayout {
    POSITION,
    POSITION_TEXCOORD,
};

struct AttributeDescriptor {
    GLuint layout;          // Layout index
    GLuint numComponents;   // Number of components
    GLenum type;            // Data type
    GLsizeiptr stride;      // Stride between attributes
    void* offset;           // Offset of the attribute
};


static const std::map<VertexLayout, std::vector<AttributeDescriptor>> layoutDescriptors = {
    { VertexLayout::POSITION, {
        {0, 3, GL_FLOAT, sizeof(float) * 3, (void*)0}
    }},
    { VertexLayout::POSITION_TEXCOORD, {
        {0, 3, GL_FLOAT, sizeof(float) * 5, (void*)0},
        {1, 2, GL_FLOAT, sizeof(float) * 5, (void*)(sizeof(float) * 3)}
    }},
};


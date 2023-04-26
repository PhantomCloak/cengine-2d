#pragma once

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

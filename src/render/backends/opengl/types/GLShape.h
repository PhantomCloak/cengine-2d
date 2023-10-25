#pragma once
#include "ebo.h"
#include "shader.h"
#include "vao.h"
#include "vbo.h"
#include <glm/glm.hpp>
#include "../../../render_primitives.h"


class GLShape {
    public:
    GLShape(float vertices[], int verticesSize, Shader* shader);
    ~GLShape();
    // void SetTexture(int texturId);
    void SetProjection(glm::mat4& projection);
    void Translate(glm::vec2 pos);
    void Scale(glm::vec2 scale);
    void Rotate(float rad);
    void SetOffset(glm::vec2 offset);
    void SetTint(glm::vec4 tint);
    void BindShape();
    void DrawShape(bool wireFrame = false);
    int verticesCount;
    void SetUV(float uStart, float vStart, float uEnd, float vEnd);

    private:
    Shader* shader;
    VBO* verticesVBO;
    VBO* uvMapVBO;
    VAO* dataFormatVAO;
};

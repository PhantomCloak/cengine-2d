#pragma once
#include "shader.h"
#include "vbo.h"
#include "vao.h"
#include "ebo.h"
#include <glm/glm.hpp>


class GLShape{
  public:
    GLShape(float vertices[], int verticesSize, Shader* shader);
    //void SetTexture(int texturId);
    void SetProjection(glm::mat4& projection);
    void Translate(glm::vec2 pos);
    void Scale(glm::vec2 scale);
    void Rotate(float rad);
    void SetOffset(glm::vec2 offset);
    void BindShape();
    void DrawShape(bool wireFrame = false);
    int verticesCount;
    private:
    Shader* shader;
    VBO* verticesVBO;
    VBO* uvMapVBO;
    VAO* dataFormatVAO;
};

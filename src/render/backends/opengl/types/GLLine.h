#include "shader.h"
#include "vbo.h"
#include "vao.h"
#include <vector>
#include <glm/glm.hpp>

class GLLine {
    public:
    GLLine(glm::vec2 start, glm::vec2 end, Shader* shader);
    void SetProjection(glm::mat4& projection);
    void Translate(glm::vec2 pos);
    void Scale(glm::vec2 scale);
    void Rotate(float rad);

    void UpdateLine(glm::vec2 start, glm::vec2 end);
    void DrawLine();

    private:
    glm::vec2 startPoint;
    glm::vec2 endPoint;
    Shader* shader;
    float vertices[4];
    VBO* verticesVBO;
    VAO* dataFormatVAO;
};

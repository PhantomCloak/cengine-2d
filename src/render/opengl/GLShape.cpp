#include "GLShape.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static float UV[] = {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f
};


GLShape::GLShape(float vertices[], int verticesSize, Shader* shaderToUse) {

    dataFormatVAO = new VAO();
    dataFormatVAO->Bind();

    this->verticesCount = verticesSize;

    assert(verticesSize != 0);

    // Initialize GPU memory
    verticesVBO = new VBO(vertices, verticesSize);
    uvMapVBO = new VBO(UV, sizeof(UV));

    dataFormatVAO->LinkAttrib(verticesVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0); // layer(0): position
    dataFormatVAO->LinkAttrib(uvMapVBO, 1, 2, GL_FLOAT, 2 * sizeof(float), (void*)0);    // layer(1): texture UV

    // Unbind memory objects
    dataFormatVAO->Unbind();
    verticesVBO->Unbind();
    uvMapVBO->Unbind();

    shader = shaderToUse;
}

void GLShape::Scale(glm::vec2 scale) {
    static unsigned int scaleLocation = glGetUniformLocation(shader->ID, "scale");
    glUniform3fv(scaleLocation, 1, glm::value_ptr(glm::vec3(scale.x, scale.y, 1)));
}

void GLShape::Translate(glm::vec2 pos) {
    static unsigned int translationLocation = glGetUniformLocation(shader->ID, "trans");
    glUniform3fv(translationLocation, 1, glm::value_ptr(glm::vec3(pos.x, pos.y, 0)));
}

void GLShape::SetProjection(glm::mat4& projection) {
    static unsigned int projectionLocation = glGetUniformLocation(shader->ID, "proj");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
}

void GLShape::SetOffset(glm::vec2 offset) {
    static unsigned int offsetLocation = glGetUniformLocation(shader->ID, "offset");
    glUniform2fv(offsetLocation, 1, glm::value_ptr(offset));
}

void GLShape::Rotate(float rad) {
    static unsigned int rotationLocation = glGetUniformLocation(shader->ID, "rot");
    glm::mat4 rot = glm::mat4(1.0f);
    rot = glm::rotate(rot, rad, glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(rotationLocation, 1, GL_TRUE, glm::value_ptr(rot));
}

void GLShape::BindShape() {
    shader->Activate();
}

void GLShape::DrawShape(bool wireFrame) {
    dataFormatVAO->Bind();
    if (wireFrame)
        glDrawArrays(GL_LINE_LOOP, 0, 4);
    else
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

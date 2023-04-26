#pragma once
#include "GLLine.h"
#include "ebo.h"
#include "shader.h"
#include "vao.h"
#include "vbo.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLLine::GLLine(glm::vec2 start, glm::vec2 end, Shader* shader) {
    startPoint = start;
    endPoint = end;

    dataFormatVAO = new VAO();
    verticesVBO = new VBO(NULL, sizeof(vertices), GL_DYNAMIC_DRAW);

    dataFormatVAO->LinkAttrib(verticesVBO, 0, 2, GL_FLOAT, 2 * sizeof(float), (void*)0);
    this->shader = shader;
}


void GLLine::UpdateLine(glm::vec2 start, glm::vec2 end) {
    vertices[0] = start.x;
    vertices[1] = start.y;
    vertices[2] = end.x;
    vertices[3] = end.y;
    verticesVBO->Update(0, sizeof(vertices), vertices);
}

void GLLine::Scale(glm::vec2 scale) {
    static unsigned int scaleLocation = glGetUniformLocation(shader->ID, "scale");
    glUniform3fv(scaleLocation, 1, glm::value_ptr(glm::vec3(scale.x, scale.y, 1)));
}

void GLLine::Translate(glm::vec2 pos) {
    static unsigned int translationLocation = glGetUniformLocation(shader->ID, "trans");
    glUniform3fv(translationLocation, 1, glm::value_ptr(glm::vec3(pos.x, pos.y, 0)));
}

void GLLine::SetProjection(glm::mat4& projection) {
    static unsigned int projectionLocation = glGetUniformLocation(shader->ID, "proj");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
}

void GLLine::Rotate(float rad) {
    static unsigned int rotationLocation = glGetUniformLocation(shader->ID, "rot");
    glm::mat4 rot = glm::mat4(1.0f);
    rot = glm::rotate(rot, rad, glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(rotationLocation, 1, GL_TRUE, glm::value_ptr(rot));
}

void GLLine::DrawLine() {
    dataFormatVAO->Bind();
    glDrawArrays(GL_LINES, 0, 2);
    dataFormatVAO->Unbind();
}

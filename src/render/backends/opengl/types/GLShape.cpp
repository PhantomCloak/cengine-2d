#include "GLShape.h"
#include "primitives.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float UV[] = {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f
};

VAO* dataFormatVAO;
VBO* interleavedVBO;

GLShape::GLShape(float vertices[], int verticesSize, Shader* shaderToUse) {
    dataFormatVAO = new VAO();
    dataFormatVAO->Bind();

    this->verticesCount = verticesSize / 3;
    assert(verticesSize != 0);

    std::vector<GLfloat> interleavedData(this->verticesCount * 5);

    for (int i = 0; i < this->verticesCount; i++) {
        interleavedData[i * 5] = vertices[i * 3];
        interleavedData[i * 5 + 1] = vertices[i * 3 + 1];
        interleavedData[i * 5 + 2] = vertices[i * 3 + 2];
        interleavedData[i * 5 + 3] = UV[i * 2];
        interleavedData[i * 5 + 4] = UV[i * 2 + 1];
    }

    interleavedVBO = new VBO(&interleavedData[0], interleavedData.size() * sizeof(float), GL_DYNAMIC_DRAW);
    dataFormatVAO->LinkAttrib(interleavedVBO, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
    dataFormatVAO->LinkAttrib(interleavedVBO, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    dataFormatVAO->Unbind();
    interleavedVBO->Unbind();

    shader = shaderToUse;
}

GLShape::~GLShape() {
    delete dataFormatVAO;
    delete interleavedVBO;
}

void GLShape::SetUV(float uStart, float vStart, float uEnd, float vEnd) {
  static float prevUV[] = {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f
  };

  float newUV[] = {
    uStart, vStart,
    uStart, vEnd,
    uEnd, vEnd,
    uEnd, vStart
  };

  if(prevUV[0] == newUV[0] && prevUV[1] == newUV[1] &&
     prevUV[2] == newUV[2] && prevUV[3] == newUV[3] &&
     prevUV[4] == newUV[4] && prevUV[5] == newUV[5] &&
     prevUV[6] == newUV[6] && prevUV[7] == newUV[7]) {
    return;
  }
  else
  {
    prevUV[0] = newUV[0];
    prevUV[1] = newUV[1];
    prevUV[2] = newUV[2];
    prevUV[3] = newUV[3];
    prevUV[4] = newUV[4];
    prevUV[5] = newUV[5];
    prevUV[6] = newUV[6];
    prevUV[7] = newUV[7];
  }

  int offset = 3 * sizeof(float);
  int stride = 5 * sizeof(float);

  for (int i = 0; i < 4; i++) {
    interleavedVBO->Update(i * stride + offset, 2 * sizeof(float), &newUV[i * 2]);
  }
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

void GLShape::SetTint(glm::vec4 tint) {
  static unsigned int tintLocation = glGetUniformLocation(shader->ID, "tint");
  glUniform4f(tintLocation, tint.x / 255, tint.y / 255, tint.z / 255, tint.w / 255);
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

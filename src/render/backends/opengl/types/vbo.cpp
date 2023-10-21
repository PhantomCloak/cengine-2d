#include "vbo.h"

VBO::VBO(GLfloat* vertices, GLsizeiptr size, unsigned int bufferMode) {
    glGenBuffers(1, &ID);
    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, vertices, bufferMode);
    Unbind();
}

VBO::~VBO() {
    Delete();
}

void VBO::Update(int offset, int size, void* vertices) {
    Bind();
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices);
}

void VBO::Bind() {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete() {
    glDeleteBuffers(1, &ID);
}


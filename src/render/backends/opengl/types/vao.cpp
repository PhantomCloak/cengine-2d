#include "vao.h"

// Constructor that generates a VAO ID
VAO::VAO() {
    glGenVertexArrays(1, &ID);
}

// Links a VBO Attribute such as a position or color to the VAO
void VAO::LinkAttrib(VBO* VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
    Bind();
    VBO->Bind();
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
    VBO->Unbind();
    Unbind();
}

void VAO::LinkAttrib(VBO* VBO, VertexLayout layout) {
    Bind();
    VBO->Bind();

    const auto& descriptors = layoutDescriptors.at(layout);
    for (const auto& desc : descriptors) {
        glVertexAttribPointer(desc.layout, desc.numComponents, desc.type, GL_FALSE, desc.stride, desc.offset);
        glEnableVertexAttribArray(desc.layout);
    }

    VBO->Unbind();
    Unbind();
}

// Binds the VAO
void VAO::Bind() {
    glBindVertexArray(ID);
}

// Unbinds the VAO
void VAO::Unbind() {
    glBindVertexArray(0);
}

// Deletes the VAO
void VAO::Delete() {
    glDeleteVertexArrays(1, &ID);
}

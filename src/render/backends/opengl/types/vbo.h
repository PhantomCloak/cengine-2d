#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>
class VBO {
    public:
    ~VBO();
    // Reference ID of the Vertex Buffer Object
    GLuint ID;
    // Constructor that generates a Vertex Buffer Object and links it to vertices
    VBO(GLfloat* vertices, GLsizeiptr size, GLenum bufferMode = GL_STATIC_DRAW);

    void Update(int offset, int size, void* vertices);
    // Binds the VBO
    void Bind();
    // Unbinds the VBO
    void Unbind();
    // Deletes the VBO
    void Delete();
};

#endif

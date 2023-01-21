#pragma once
#include "glm/glm.hpp"

const glm::vec2 TriangleShape[3] = {
    glm::vec2(0.0, 0.0),
    glm::vec2(1.0, 0.0),
    glm::vec2(0.5, 1.0),
};

const glm::vec2 CubeShape[4] = {
    glm::vec2(0.0, 0.0),
    glm::vec2(1.0, 0),
    glm::vec2(1.0, 1.0),
    glm::vec2(0.0, 1.0)
};

struct Mesh {
    glm::vec2 vertices[4] = {
        glm::vec2(0.0, 0.0),
        glm::vec2(2.0, 0),
        glm::vec2(2.0, 1.0),
        glm::vec2(0.0, 1.0)
    };
    int verticiesCount;

    Mesh() {
        // if(verticiesCount > sizeof(vertices))
        // assert("err too long");
        // this->vertices = CubeShape;
        this->verticiesCount = 4;
    }
};

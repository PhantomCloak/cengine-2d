#version 330 core

//Positions/Coordinates
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;


// Outputs the color for the Fragment Shader
out vec2 texCoord;
// Controls the scale of the vertices
uniform mat4 proj;
uniform vec3 trans;
uniform vec3 scale;


void main()
{
        gl_Position = proj * vec4(aPos * scale + trans, 1.0);
        texCoord = aTex;
}

#version 330 core

//Positions/Coordinates
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;


// Outputs the color for the Fragment Shader
out vec2 texCoord;
out vec2 texOffset;

// Controls the scale of the vertices
uniform mat4 proj;
uniform vec3 trans;
uniform vec3 scale;
uniform vec2 offset;


void main()
{
        //gl_Position = proj * vec4(aPos * scale + trans, 1.0);
        gl_Position = proj * (rot * vec4(aPos * scale, 1.0)) + vec4(trans, 0.0);
        texCoord = aTex;
        textOffset = offset;
}

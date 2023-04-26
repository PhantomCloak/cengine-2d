#version 330 core
layout (location = 0) in vec2 vertex; // <vec2 pos, vec2 tex>
out vec4 Color;

uniform mat4 projection;
uniform vec4 color;

void main()
{
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    Color = color;
}

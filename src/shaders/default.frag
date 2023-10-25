#version 330 core

out vec4 FragColor;
in vec2 texCoord;
in vec2 texOffset;

uniform sampler2D tex0;
uniform vec4 tint;

void main()
{
    vec4 sampledColor = texture(tex0, texCoord + texOffset);
    FragColor = sampledColor * tint; // apply the tint by multiplying
}

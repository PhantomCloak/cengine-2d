#version 330 core

out vec4 FragColor;
in vec2 texCoord;
in vec2 texOffset;

uniform sampler2D tex0;

void main()
{
	FragColor = texture(tex0, texCoord + texOffset);
}

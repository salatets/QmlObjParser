#version 330 core
out vec4 color;

uniform sampler2D diffuse;

in vec2 TexCoords;

void main()
{
    color = vec4(texture(diffuse, TexCoords).rgb, 1.0f);
}

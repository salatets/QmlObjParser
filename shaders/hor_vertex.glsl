#version 330 core
in vec3 position;
in vec3 color;

out vec3 Color;

uniform mat4 model;
uniform float point;

void main()
{
    // projection * view *
    gl_Position = model * vec4(position, 1.0f);

    gl_PointSize = 4;

    Color = color;
}

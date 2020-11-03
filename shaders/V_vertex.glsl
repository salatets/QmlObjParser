#version 330 core
in vec3 position;

uniform mat4 model;
//uniform mat4 projection;

void main()
{
    // projection * view *
    gl_Position = model * vec4(position, 1.0f);
}

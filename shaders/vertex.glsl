#version 420

uniform mat4 model;
in vec3 position;

void main() {
    // does not alter the verticies at all
    gl_Position = model*vec4(position, 1);
}

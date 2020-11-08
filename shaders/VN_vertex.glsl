#version 330 core
in vec3 position;
in vec3 normal;
in vec2 texCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
//uniform mat4 projection;

void main()
{
    // projection * view *
    gl_Position = model * vec4(position, 1.0f);
    FragPos = vec3(model * vec4(position, 1.0f));
    Normal = mat3(transpose(inverse(model))) * normal;
    TexCoords = texCoords;
}

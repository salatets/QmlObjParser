#version 330 core
out vec4 color;

struct Material {
    vec3 specular;
    float shininess;
};

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform Material material;

void main()
{
    // Ambient
    float ambientStrength = 0.2f;
    vec3 ambient = ambientStrength * vec3(139,0,225);

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(139,0,225);

    // Specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = specularStrength * spec * material.specular;

    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0f);
}

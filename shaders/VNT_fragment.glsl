#version 330 core
out vec4 color;

struct Material {
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};


in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform Material material;

void main()
{
    // Ambient
    float ambientStrength = 0.2f;
    vec3 ambient = ambientStrength * texture(material.diffuse, TexCoords).rgb;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * texture(material.diffuse, TexCoords).rgb;

    // Specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = specularStrength * spec * material.specular;

    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0f);
//    color = vec4(texture(diffuse, TexCoords).rgb, 1); //vec4(TexCoords,1,1); //

}

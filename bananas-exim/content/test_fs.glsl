#version 400

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec3 FragPos;
in vec3 normal;
in vec2 uv;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;


void main() 
{
    // Ambient lighting
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, uv));

    // Diffuse map/lighting (max() so there are no negative values)
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - norm);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, uv));

    // Specular lighting
    vec3 viewDir    = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, uv));

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
#version 400

out vec4 FragColor;

in vec3 FragPos;
in vec3 normal;
in vec2 uv;

uniform sampler2D texture1;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main() 
{
    // Ambient lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting (max() so there are no negative values)
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - norm);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular lighting
    float specularStrength = 0.5;
    vec3 viewDir    = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 color = (ambient + diffuse + specular) * texture(texture1, uv).rgb;
    FragColor = vec4(color, 1.0);
}
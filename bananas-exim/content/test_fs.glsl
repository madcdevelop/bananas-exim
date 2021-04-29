#version 400

out vec4 FragColor;

in vec3 FragPos;
in vec3 normal;
in vec2 uv;

uniform sampler2D texture1;

uniform vec3 lightPos;
uniform vec3 lightColor;

void main() 
{
    // ambient lighting
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse lighting (max() for negative values)
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - norm);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 color = (ambient * diffuse) * texture(texture1, uv).rgb;
    FragColor = vec4(color, 1.0);
}
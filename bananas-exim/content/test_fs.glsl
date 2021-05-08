#version 400

#define NR_POINT_LIGHTS 4

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
    float cutoff;
    float outerCutOff;
};

// outputs
out vec4 FragColor;

// inputs
in vec3 FragPos;
in vec3 normal;
in vec2 uv;

// uniforms
uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

// function prototypes
vec3 CalculateDirectionalLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() 
{
    // properties
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // directional light
    vec3 result = CalculateDirectionalLight(dirLight, norm, viewDir);

    // point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalculatePointLight(pointLights[i], norm, FragPos, viewDir);

    // spot light
    result += CalculateSpotLight(spotLight, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 CalculateDirectionalLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, uv));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, uv));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, uv));
    return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, uv));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, uv));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, uv));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // spotlight (soft edges)
    float theta     = dot(lightDir, normalize(-light.direction));
    float epsilon   = (light.cutoff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // combine results
    vec3 ambient  = light.ambient * vec3(texture(material.diffuse, uv));
    vec3 diffuse  = light.diffuse * diff * vec3(texture(material.diffuse, uv));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, uv));
    ambient *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    diffuse  *= intensity;
    specular *= intensity;

    return (ambient + diffuse + specular);
}
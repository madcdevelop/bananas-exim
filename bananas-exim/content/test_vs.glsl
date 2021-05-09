#version 400    

layout(location = 0) in vec3 vertexPositionLocal;
layout(location = 1) in vec3 normalPositionLocal;
layout(location = 2) in vec2 vertexUV;

out vec3 FragPos;
out vec3 Normal;
out vec2 UV;

uniform mat4 MVP;
uniform mat4 model;
uniform mat3 normalMatrix;

void main() {
    gl_Position = MVP * vec4(vertexPositionLocal,1.0);
    
    FragPos = vec3(model * vec4(vertexPositionLocal, 1.0)); 

    Normal = normalMatrix * normalPositionLocal;

    UV = vertexUV;
}
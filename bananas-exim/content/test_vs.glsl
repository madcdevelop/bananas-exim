#version 400    

layout(location = 0) in vec3 vertexPositionLocal;
layout(location = 1) in vec3 normalPositionLocal;
layout(location = 2) in vec2 vertexUV;

out vec3 FragPos;
out vec3 normal;
out vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(vertexPositionLocal,1.0);
    
    FragPos = vec3(model * vec4(vertexPositionLocal, 1.0)); 

    // Normal Matrix: very costly on the gpu. For performance move this  
    // op to cpu and send via a uniform
    normal = mat3(transpose(inverse(model))) * normalPositionLocal;

    uv = vertexUV;
}
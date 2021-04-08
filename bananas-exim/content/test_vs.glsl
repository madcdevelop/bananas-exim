#version 400    

layout(location = 0) in vec3 vertexPositionModelSpace;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(vertexPositionModelSpace,1);
}
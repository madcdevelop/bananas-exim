#version 400

in vec2 uv;
in vec3 fragmentColor;

out vec3 color;

uniform sampler2D myTextureSampler;

void main() {

    color = texture(myTextureSampler, uv).rgb;
}
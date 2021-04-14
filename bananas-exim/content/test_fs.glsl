#version 400

in vec2 uv;

out vec3 color;

uniform sampler2D texture1;

void main() {

    color = texture(texture1, uv).rgb;
}
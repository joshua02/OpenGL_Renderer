#version 460 core
layout (location = 0) in vec3 aPos;
uniform vec2 tfPos;
out vec4 vertexColor;
void main() {
    gl_Position = vec4(aPos.x + tfPos.x, aPos.y + tfPos.y, 1.0, 1.0);
    vertexColor = vec4(0.5, 0.0, 0.0, 1.0);
}
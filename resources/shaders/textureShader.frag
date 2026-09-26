#version 460 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec4 tintColor;

void main() {
    vec4 text = texture(ourTexture, TexCoord);
//    if (text.a < 0.1) {
//        discard;
//    }
    FragColor = vec4(vec3(text) * vec3(tintColor), text.a);
}
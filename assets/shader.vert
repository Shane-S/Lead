#version 330

precision mediump float;

uniform mat4 mvp;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoordIn;

// Get the information for the fragment shader
smooth out vec2 texCoordOut;

void main() {
    texCoordOut = texCoordIn;
    vec4 pos = mvp * vec4(position.xyz, 1.0f);
    gl_Position = pos;
}
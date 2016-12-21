#version 300 es

precision mediump float;

uniform mat4 mvp;

layout (location = 0) in vec2 position;
layout (location = 2) in vec2 texCoordIn;

// Get the information for the fragment shader
smooth out vec2 texCoordOut;

void main() {
    texCoordOut = texCoordIn;
    gl_Position = mvp * vec4(position.xy, 1.f, 1.0f);
}

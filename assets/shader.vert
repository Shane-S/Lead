#version 120

precision mediump float;

uniform mat4 mvp;

attribute vec3 position;
attribute vec3 normal;
attribute vec2 texCoordIn;

// Get the information for the fragment shader
varying vec2 texCoordOut;

void main() {
    texCoordOut = texCoordIn;
    vec4 pos = mvp * vec4(position.xyz, 1.0f);
    gl_Position = pos;
}

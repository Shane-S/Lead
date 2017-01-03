#version 120

precision mediump float;

uniform mat4 mvp;

attribute vec2 position;

attribute vec2 texCoordIn;
varying vec2 texCoordOut;

void main() {
    texCoordOut = texCoordIn;
    gl_Position = mvp * vec4(position.xy, 1.0f, 1.0f);
}

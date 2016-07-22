#version 330
precision mediump float;

/* set up a uniform sampler2D to get texture */
uniform sampler2D tex;

smooth in vec2 texCoordOut;
out vec4 colour;
void main()
{
    colour = texture2D(tex, texCoordOut);   
}

#version 120

/* set up a uniform sampler2D to get texture */
uniform sampler2D tex;

varying highp vec2 texCoordOut;

void main()
{
    gl_FragColor = texture2D(tex, texCoordOut); //vec4(texCoordOut.s, texCoordOut.t, 1.0f, 1.0f); //
}

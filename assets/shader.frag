//
//  Shader.fsh
//  Assignment2
//
//  Created by Shane Spoor on 2015-02-26.
//  Copyright (c) 2015 BCIT. All rights reserved.
//
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

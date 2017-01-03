//
//  Shader.fsh
//  Assignment2
//
//  Created by Shane Spoor on 2015-02-26.
//  Copyright (c) 2015 BCIT. All rights reserved.
//
#version 120

/* set up a uniform sampler2D to get texture */
uniform sampler2D tex;

varying vec2 texCoordOut;
void main()
{
    gl_FragColor = texture2D(tex, texCoordOut);   
}

//  Stolen/adapted from Viking XII: https://github.com/LamePancake/VikingXII

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <SDL2/SDL_image.h>

#ifdef _WIN32
#include <Windows.h>
#endif

#include "GLProgramUtils.h"

using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;
using std::stringbuf;

bool validateProgram(GLuint prog)
{
    GLint logLength, status;
    
    glValidateProgram(prog);
    
#if defined(DEBUG)
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        cout << "Program validate log:\n%s" << log << endl;
        free(log);
    }
#endif // defined(DEBUG)
    
    // status == 0 means it failed validation
    glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
    return status;
}

bool linkProgram(GLuint prog)
{
    GLint status;
    glLinkProgram(prog);

#if defined(DEBUG)
    // Output info about the program linking
    GLint logLength;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        cout << "Program link log:\n%s" << log << endl;
        free(log);
    }
#endif // defined(DEBUG)

    // status == 0 means it failed linking
    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    return status;
}

bool compileShader(GLuint *shader, GLenum type, const char* file)
{
    GLint status;
    const GLchar *source;
    
    // Read in the shader file as a string
    ifstream in(file);
    std::stringstream buffer;
    buffer << in.rdbuf();
    
    if(!buffer) {
        cerr << "Failed to load vertex shader" << endl;
        return false;  
    }
    
    std::string srcStr(buffer.str());
    source = (GLchar*)srcStr.c_str();
    
    // Create and attempt to compile the shader with the given source code
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);
    
#ifndef NDEBUG
    // Output info about the shader compilation
    GLint logLength;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)::malloc(logLength);
        glGetShaderInfoLog(*shader, logLength, &logLength, log);
        cout << "Shader compile log:\n" << log << endl;
        ::free(log);
    }
#endif
    
    // Verify that the shader compiled correctly
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        glDeleteShader(*shader);
        return false;
    }
    return true;
}

int makeProgram(GLuint *program, const char* vertPath, const char* fragPath, ShaderAttribute* attributes, int attrCount) {
    GLuint vertShader, fragShader;
    
    // Create shader program.
    *program = glCreateProgram();
    
    // Compile the shaders (or don't)
    if (!compileShader(&vertShader, GL_VERTEX_SHADER, vertPath)) {
        cerr << "Could not compile vertex shader " << vertPath << endl;
        return 1;
    }
    if (!compileShader(&fragShader, GL_FRAGMENT_SHADER, fragPath)) {
        cerr << "Could not compile fragment shader " << fragPath << endl;
        glDeleteShader(vertShader);
        return 2;
    }
    
    // Attach vertex shader to program.
    glAttachShader(*program, vertShader);
    
    // Attach fragment shader to program.
    glAttachShader(*program, fragShader);
    
    // Bind attribute locations.
    // This needs to be done prior to linking
    for(int i = 0; i < attrCount; i += 2) {
        glBindAttribLocation(*program, attributes[i].index, attributes[i].attributeName);
    }
    
    // Link program.
    if (!linkProgram(*program)) {
        cerr << "Could not link program for vertex shader " << vertPath << " and fragment shader " << fragPath << endl;

        if (vertShader) {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader) {
            glDeleteShader(fragShader);
            fragShader = 0;
        }
        if (*program) {
            glDeleteProgram(*program);
            *program = 0;
        }
        
        return 3;
    }
    
    // Release vertex and fragment shaders.
    if (vertShader) {
        glDetachShader(*program, vertShader);
        glDeleteShader(vertShader);
    }
    if (fragShader) {
        glDetachShader(*program, fragShader);
        glDeleteShader(fragShader);
    }
    
    return 0;
}

/**
 * Determines the image's format and an appropriate format to store the image in OpenGL.
 * @param img       The SDL_Surface containing the texture.
 * @param imgFormat Holds the OpenGL pixel format describing the image in client memory.
 * @param intFormat Holds the OpenGL pixel format that will be used for storage on the GPU.
 * @return True if there was an appropriate format for the image, false otherwise (requires conversion to some other format).
 */
bool getGLFormat(SDL_Surface* img, GLenum* imgFormat, GLenum* intFormat) {
	switch (img->format->format) {
	case SDL_PIXELFORMAT_ABGR8888:
		*imgFormat = GL_RGBA;
        *intFormat = GL_RGBA8;
        return true;
	case SDL_PIXELFORMAT_BGR888:
		*imgFormat = GL_RGB;
        *intFormat = GL_RGB8;
        return true;
	default:
		return false;
	}
}

// Load in and set up texture image (adapted from Ray Wenderlich)
bool setupTexture(char const* file, GLuint* out)
{
	SDL_Surface* img = IMG_Load(file);
    
	if (!img) {
		cerr << "Error while loading texture " << file << ": " << IMG_GetError();
		return false;
	}

	GLuint tex;
    GLenum imgFormat, intFormat;
	if (!getGLFormat(img, &imgFormat, &intFormat)) {
        std::cout << "Changing from pixel format " << SDL_GetPixelFormatName(img->format->format) << " to SDL_PIXELFORMAT_ABGR8888" << std::endl;
        
        // SDL_PIXELFORMAT_ABGR because SDL manipulates things as Uint32; on little-endian machines, this stores each pixel backwards
        SDL_Surface* old = img;
        img = SDL_ConvertSurfaceFormat(old, SDL_PIXELFORMAT_ABGR8888, 0);
        imgFormat = GL_RGBA;
        intFormat = GL_RGBA8;
        SDL_FreeSurface(old);
	}

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // This probably isn't really necessary
	glTexImage2D(GL_TEXTURE_2D, 0, intFormat, img->w, img->h, 0, imgFormat, GL_UNSIGNED_BYTE, img->pixels);

	SDL_FreeSurface(img);
	*out = tex;
    return true;
}

void setVertexAttributes(VertexAttribute* attributes, unsigned int numAttrs)
{
    // Loop through all attributes and enable them
    for(unsigned i = 0; i < numAttrs; i++)
    {
        glEnableVertexAttribArray(attributes[i].index);
        glVertexAttribPointer(attributes[i].index,
                              attributes[i].size,
                              attributes[i].type,
                              attributes[i].normalise,
                              attributes[i].stride,
                              BUFFER_OFFSET(attributes[i].bufferOffset));
    }
}
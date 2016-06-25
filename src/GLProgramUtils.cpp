//  Stolen/adapted from Viking XII: https://github.com/LamePancake/VikingXII

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "GLProgramUtils.h"
#include <SDL2/SDL_image.h>

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
    
    source = (GLchar*)buffer.str().c_str();
    
    // Create and attempt to compile the shader with the given source code
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);
    
#if defined(DEBUG)
    // Output info about the shader compilation
    GLint logLength;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(*shader, logLength, &logLength, log);
        cout << "Shader compile log:\n" << log << endl;
        free(log);
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
    if (compileShader(&fragShader, GL_FRAGMENT_SHADER, fragPath)) {
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
    if (linkProgram(*program)) {
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
 * I'm probably not actually going to bother switching up the formats, mainly because that would be a giant
 * (and unnecessary in most cases) pain in the ass.
 */
GLenum getGLFormat(SDL_Surface* img) {
	switch (img->format->format) {
	case SDL_PIXELFORMAT_RGBA4444:
		return GL_RGBA;
	case SDL_PIXELFORMAT_RGB444:
		return GL_RGB;
	case SDL_PIXELFORMAT_BGRA4444:
		return GL_BGRA;
	default:
		return -1;
	}
}

// Load in and set up texture image (adapted from Ray Wenderlich)
bool setupTexture(char* file, GLuint* out)
{
	SDL_Surface* img = IMG_Load(file);
	if (!img) {
		cerr << "Error while loading texture " << file << ": " << IMG_GetError();
		return false;
	}

	GLuint tex;
	GLenum format = getGLFormat(img);
	if (format == -1) {
		cerr << "Texture is not in a supported image format (so now you have to implement the conversion, lol)" << endl;
		SDL_FreeSurface(img);
		return false;
	}

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // This probably isn't really necessary
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 0, format, GL_UNSIGNED_BYTE, img->pixels);

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
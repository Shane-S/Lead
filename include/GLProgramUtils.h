#pragma once

#include <glad/glad.h>

//  Adapted from VikingXII: https://github.com/LamePancake/VikingXII

// Converts a buffer offset to a pointer to make OpenGL happy.
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

typedef enum VertexAttribIndex {
    VertexAttribPos,
    VertexAttribNormal,
    VertexAttribTexCoord0,
    VertexAttribTexCoord1
} VertexAttribIndex;

/**
 * Contains the OpenGL index indicating the type of attribute and its name within the vertex shader.
 */
typedef struct _ShaderAttribute {
	VertexAttribIndex index; // The user-defined attribute index to reference this attribute (e.g. GLKVertexAttribPosition).
    const char *attributeName;  // The name of the attribute within the shader (e.g. "texCoordIn").
} ShaderAttribute;

typedef struct _VertexAttribute {
	VertexAttribIndex index;    // The user-defined attribute index to reference this attribute (e.g. GLKVertexAttribPosition).
    GLint size;                 // The number of elements that make up this attribute.
    GLenum type;                // The type of element (GL_FLOAT, GL_UNSIGNED_SHORT, etc.).
    GLboolean normalise;        // Whether to normalise the values before use.
    GLsizei stride;             // The number of bytes between attributes of this type.
    unsigned int bufferOffset;  // The starting offset into the buffer from which to read the first attribute.
} VertexAttribute;

/**
 * @brief Validates a linked program.
 *
 * @discussion According to the <a href="https://www.khronos.org/opengles/sdk/docs/man/xhtml/glValidateProgram.xml">
 *             OpenGL</a> specification, glValidateProgram determines whether the program can run given the current
 *             state of OpenGL. If not, use <a href="https://www.khronos.org/opengles/sdk/docs/man/xhtml/glGetProgramInfoLog.xml">
 *             glGetProgramInfoLog</a> to retrieve a string describing the problem.
 *
 * @param prog Handle to the program to be validated.
 * @return Whether the program is valid.
 */
bool validateProgram(GLuint prog);

/**
 * @brief Links a compiled shader program.
 * @param prog Index to the compiled program to be linked.
 * @return Whether the shader was successfully linked.
 */
bool linkProgram(GLuint prog);

/**
 * @brief Compiles the shader of the specified type with source code in the given file.
 *
 * @discussion Note that the shader can only be used if this function returns YES.
 *
 * @param shader Pointer to an integer which will hold the shader index on success.
 * @param type   The type of shader (not sure what this actually is).
 * @param file   The path to the file containing the shader's source code.
 * @return       Whether the shader was successfully compiled.
 */
bool compileShader(GLuint* shader, GLenum type, const char* file);

/**
 * @brief Compiles, links and validates a program with the given vertex and fragment shader.
 * @discussion The returned integer will be 0 on success and will otherwise hold the creation step that failed.
 * @param program     Pointer to ta GLuint which will hold the program handle on successful compilation.
 * @param vertPath    Path to the vertex shader.
 * @param fragPath    Path to the fragment shader.
 * @param attributes  An array of shader attribute structs.
 * @return 0 on success, 1 if compiling/attaching the vertex shader failed, 2 if compiling/attaching the fragment
 *         shader failed, 3 if linking the program failed.
 */
int makeProgram(GLuint* program, const char* vertPath, const char* fragPath, ShaderAttribute* attributes, int attrCount);

/**
 * @brief Loads in a texture.
 * @param file The path to the texture to load.
 * @param out  Variable that will contain the texture index on successful loading.
 * @return True on success, false on failure.
 */
bool setupTexture(char const* file, GLuint* out);

/**
 * @brief Sets up the vertex specification for the currently bound buffer.
 * @param attributes A list of vertex attributes to enable.
 * @param numAttrs   The number of attributes in the list.
 */
void setVertexAttributes(VertexAttribute* attributes, unsigned int numAttrs);

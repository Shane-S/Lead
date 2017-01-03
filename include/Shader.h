#pragma once
#include <glad/glad.h>
#include <string>
#include <map>
#include <vector>
#include "GLProgramUtils.h"

class Shader {
public:
	/**
	 * @brief Constructs a shader program with the specified vertex and fragment shaders and their corresponding variables.
	 *
	 * @param attrs          The names and types (e.g. position, normal, etc.) of attribute variables in the vertex shader.
	 * @param uniforms       The names of uniform variables in the shaders.
	 * @param vertShaderPath The path to the vertex shader.
	 * @param fragShaderPath The path the fragment shader.
	 */
	Shader(std::vector<ShaderAttribute> attrs, std::vector<std::string> uniforms, std::string vertShaderPath, std::string fragShaderPath);
	Shader(Shader&& other);
	Shader& operator=(Shader&& rhs);
	~Shader();
	
	// We only want move semantics for this object to avoid having the program accidentally deleted
	Shader(Shader const& other) = delete;
	Shader& operator=(Shader const& rhs) = delete;

	/**
	 * Returms the map of uniform variable names to the OpenGL identifiers.
	 * @return The map of uniform names -> OpenGL identifiers.
	 */
	const std::map<std::string, GLuint>& getUniforms() const;
	
	/**
	 * Determines whether this shader is the one currently loaded in OpenGL.
	 * @return Whether this shader is the active OpenGL shader.
	 */
	bool isCurrent() const;

	/**
	 * Sets this program as the active shader program.
	 */
	void makeCurrent() const;
	
private:
	std::map<std::string, GLuint> uniforms_;
	GLuint program_;
};
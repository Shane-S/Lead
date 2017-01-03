#include <sstream>
#include <string>
#include "Shader.h"

Shader::Shader(std::vector<ShaderAttribute> attrs, std::vector<std::string> uniforms, std::string vertShaderPath, std::string fragShaderPath)
	: uniforms_()
{
	int result = makeProgram(&program_, vertShaderPath.c_str(), fragShaderPath.c_str(), &attrs[0], attrs.size());
	if (result) {
		std::stringstream stream;
		stream << "Couldn't create shader program (makeProgram returned " << result << ", gl error was " << glGetError() << ").";
		throw stream.str();
	}

	// Get the locations of all uniform variables for setting later
	for (auto& uniform : uniforms) {
		GLuint index = glGetUniformLocation(program_, uniform.c_str());
		uniforms_[uniform] = index;
	}
}

Shader::Shader(Shader && other)
	: uniforms_(std::move(other.uniforms_)), program_(other.program_)
{
	other.program_ = 0; // From the OpenGL spec for glProgramDelete, "A value of 0 for program will be silently ignored."
}

Shader & Shader::operator=(Shader && rhs)
{
	this->uniforms_ = std::move(rhs.uniforms_);
	this->program_ = rhs.program_;
	rhs.program_ = 0;
	return *this;
}

Shader::~Shader()
{
	glDeleteProgram(program_);
}

const std::map<std::string, GLuint>& Shader::getUniforms() const
{
	return uniforms_;
}

bool Shader::isCurrent() const
{
	GLint curProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &curProgram);
	return curProgram == program_;
}

void Shader::makeCurrent() const
{
	glUseProgram(program_);
}

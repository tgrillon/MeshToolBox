#include "Core/Renderer/Shader.h"

#include "Core/PrintHelpers.h"

#include <glad/gl.h>

#include <fstream>
#include <iostream>
#include <vector>

namespace Renderer
{

static std::string ReadTextFile(const std::filesystem::path& path)
{
	std::ifstream file(path);

	if(!file.is_open())
	{
		Error("Failed to open file: {}", path.string());
		return {};
	}

	std::ostringstream contentStream;
	contentStream << file.rdbuf();
	return contentStream.str();
}

uint32_t CreateComputeShader(const std::filesystem::path& path)
{
	std::string shaderSource = ReadTextFile(path);

	GLuint shaderHandle = glCreateShader(GL_COMPUTE_SHADER);

	const GLchar* source = static_cast<const GLchar*>(shaderSource.c_str());
	glShaderSource(shaderHandle, 1, &source, 0);

	glCompileShader(shaderHandle);

	GLint isCompiled = 0;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(shaderHandle, maxLength, &maxLength, &infoLog[0]);

		Error("{}", infoLog.data());

		glDeleteShader(shaderHandle);
		return -1;
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, shaderHandle);
	glLinkProgram(program);

	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, static_cast<int*>(&isLinked));
	if(isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		Error("{}", infoLog.data());

		glDeleteProgram(program);
		glDeleteShader(shaderHandle);

		return -1;
	}

	glDetachShader(program, shaderHandle);
	return program;
}

uint32_t ReloadComputeShader(uint32_t shaderHandle, const std::filesystem::path& path)
{
	uint32_t newShaderHandle = CreateComputeShader(path);

	// Return old shader if compilation failed
	if(newShaderHandle == -1)
		return shaderHandle;

	glDeleteProgram(shaderHandle);
	return newShaderHandle;
}

uint32_t CreateGraphicsShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
{
	std::string vertexShaderSource = ReadTextFile(vertexPath);
	std::string fragmentShaderSource = ReadTextFile(fragmentPath);

	// Vertex shader

	GLuint vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);

	const GLchar* source = static_cast<const GLchar*>(vertexShaderSource.c_str());
	glShaderSource(vertexShaderHandle, 1, &source, 0);

	glCompileShader(vertexShaderHandle);

	GLint isCompiled = 0;
	glGetShaderiv(vertexShaderHandle, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShaderHandle, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShaderHandle, maxLength, &maxLength, &infoLog[0]);

		Error("{}", infoLog.data());

		glDeleteShader(vertexShaderHandle);
		return -1;
	}

	// Fragment shader

	GLuint fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);

	source = static_cast<const GLchar*>(fragmentShaderSource.c_str());
	glShaderSource(fragmentShaderHandle, 1, &source, 0);

	glCompileShader(fragmentShaderHandle);

	isCompiled = 0;
	glGetShaderiv(fragmentShaderHandle, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShaderHandle, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShaderHandle, maxLength, &maxLength, &infoLog[0]);

		Error("{}", infoLog.data());

		glDeleteShader(fragmentShaderHandle);
		return -1;
	}

	// Program linking

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShaderHandle);
	glAttachShader(program, fragmentShaderHandle);
	glLinkProgram(program);

	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, static_cast<int*>(&isLinked));
	if(isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		Error("{}", infoLog.data());

		glDeleteProgram(program);
		glDeleteShader(vertexShaderHandle);
		glDeleteShader(fragmentShaderHandle);

		return -1;
	}

	glDetachShader(program, vertexShaderHandle);
	glDetachShader(program, fragmentShaderHandle);
	return program;
}

uint32_t ReloadGraphicsShader(
	uint32_t shaderHandle, const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
{
	uint32_t newShaderHandle = CreateGraphicsShader(vertexPath, fragmentPath);

	// Return old shader if compilation failed
	if(newShaderHandle == -1)
		return shaderHandle;

	glDeleteProgram(shaderHandle);
	return newShaderHandle;
}

int GetUniformLocation(const char* name, int program)
{
	int location = glGetUniformLocation(program, name);
	return location;
}

} // namespace Renderer

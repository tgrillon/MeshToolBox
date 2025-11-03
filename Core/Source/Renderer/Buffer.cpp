#include "Core/Renderer/Buffer.h"

#include <glad/gl.h>

namespace Renderer
{
void LoadBuffer(int vbo, int location, int size, const std::vector<float>& data, unsigned int mode)
{
	auto byteCount = sizeof(float) * data.size();

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, byteCount, data.data(), mode);

	glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, size * sizeof(float), (const void*)0);
	glEnableVertexAttribArray(location);
}

void LoadBuffer(int vbo, int location, int size, const std::vector<int>& data, unsigned int mode)
{
	auto byteCount = sizeof(int) * data.size();

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, byteCount, data.data(), mode);

	glVertexAttribIPointer(location, size, GL_INT, size * sizeof(int), (const void*)0);
	glEnableVertexAttribArray(location);
}

// Load a VBO for an instance vertex attribute
void LoadBufferInstance(int vbo, int location, int size, const std::vector<float>& data, unsigned int mode)
{
	LoadBuffer(vbo, location, size, data, mode);

	glVertexAttribDivisor(location, 1);
}

// Load a VBO for an instance vertex attribute
void LoadBufferInstance(int vbo, int location, int size, const std::vector<int>& data, unsigned int mode)
{
	LoadBuffer(vbo, location, size, data, mode);

	glVertexAttribDivisor(location, 1);
}

void UpdateBuffer(int vbo, const std::vector<float>& data)
{
	auto byteCount = sizeof(float) * data.size();

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, byteCount, data.data());
}

void UpdateBuffer(int vbo, const std::vector<int>& data)
{
	auto byteCount = sizeof(int) * data.size();

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, byteCount, data.data());
}
} // namespace Renderer

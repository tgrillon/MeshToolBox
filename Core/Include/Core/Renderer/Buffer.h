#pragma once

#include <glad/gl.h>

#include <vector>

namespace Renderer
{
void LoadBuffer(int vbo, int location, int size, const std::vector<float>& data, unsigned int mode = GL_STATIC_DRAW);
void LoadBuffer(int vbo, int location, int size, const std::vector<int>& data, unsigned int mode = GL_STATIC_DRAW);
void LoadBufferInstance(int vbo, int location, int size, const std::vector<float>& data, unsigned int mode = GL_STATIC_DRAW);
void LoadBufferInstance(int vbo, int location, int size, const std::vector<int>& data, unsigned int mode = GL_STATIC_DRAW);
void UpdateBuffer(int vbo, const std::vector<float>& data);
void UpdateBuffer(int vbo, const std::vector<int>& data);
} // namespace Renderer

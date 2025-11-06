#pragma once

#include "Core/BaseTypes.h"

#include <filesystem>

namespace App
{
class MeshBuffer
{
public:
	struct TriangleData
	{
		Core::Container::DynamicArray<Core::BaseType::Vec3> VertexPositions;
		Core::Container::DynamicArray<Core::BaseType::Vec2> VertexTexCoords;
		Core::Container::DynamicArray<Core::BaseType::Vec3> VertexNormals;

		bool HasTexCoords() { return VertexTexCoords.size() > 0; }

		bool HasNormal() { return VertexNormals.size() > 0; }
	};

public:
	MeshBuffer() = default;
	MeshBuffer(const std::filesystem::path& filepath, bool computeNormals = false);

private:
	TriangleData m_Data;
};
} // namespace App

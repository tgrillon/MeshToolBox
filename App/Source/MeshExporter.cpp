#include "Application/MeshExporter.h"

#include "Application/ExtraDataContainer.h"
#include "Application/ExtraDataType.h"
#include "Application/PrimitiveProxy.h"
#include "Core/BaseTypes.h"
#include "Core/PrintHelpers.h"

using namespace Data::Surface;
using namespace Data::Primitive;
using namespace Data::ExtraData;
using namespace Utilitary::Primitive;
using namespace Core::BaseType;

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <unordered_set>

namespace Utilitary::Surface
{
void MeshExporter::ExportOFF(const Mesh& mesh, const std::filesystem::path& filepath)
{
	std::ofstream file(filepath, std::ios::trunc);

	Debug("Writing to {}", filepath.string());

	// Write the OFF header.
	file << "OFF" << '\n';

	// Write the number of vertices, faces, and edges (0 for edges as per OFF format).
	file << mesh.GetVertexCount() << ' ' << mesh.GetTriangleCount() << ' ' << 0 << '\n';

	// Write vertex positions.
	for(auto&& curVertex : mesh.m_Vertices)
		file << curVertex.Position.x << ' ' << curVertex.Position.y << ' ' << curVertex.Position.z << '\n';

	// Write triangle definitions.
	for(auto&& curFace : mesh.m_Triangles)
		file << curFace.Vertices.size() << ' ' << (curFace.Vertices[0] + 1) << ' ' << (curFace.Vertices[1] + 1) << ' '
			 << (curFace.Vertices[2] + 1) << '\n';

	file.close();
}

void MeshExporter::ExportOBJ(const Mesh& mesh, const std::filesystem::path& filepath)

{
	std::ofstream file(filepath, std::ios::trunc);

	Debug("Writing to {}", filepath.string());

	// Write vertex positions.
	for(auto&& curVertex : mesh.m_Vertices)
		file << 'v' << ' ' << curVertex.Position.x << ' ' << curVertex.Position.y << ' ' << curVertex.Position.z << '\n';

	// Write texture coordinates if available.

	std::vector<Vec2> uniqueTexCoords;
	if(mesh.HasTrianglesExtraDataContainer() && mesh.m_TrianglesExtraDataContainer[0].Has<VerticesTexCoordsExtraData>())
	{
		std::unordered_set<Vec2> texCoords;
		for(auto&& curTriangleED : mesh.m_TrianglesExtraDataContainer)
		{
			auto texCoordsED = curTriangleED.Get<VerticesTexCoordsExtraData>();
			assert(texCoordsED != nullptr && "All vertices must have texture coordinates if one has it");
			for(auto&& curTexCoords : texCoordsED->GetData())
			{
				texCoords.insert(curTexCoords);
			}
		}

		uniqueTexCoords.insert(uniqueTexCoords.begin(), texCoords.begin(), texCoords.end());
		for(auto&& curTexCoords : uniqueTexCoords)
		{
			file << "vt" << ' ' << curTexCoords.x << ' ' << curTexCoords.y << '\n';
		}
	}

	bool hasTexCoords = !uniqueTexCoords.empty();

	// Write face normals if available.
	std::vector<Vec3> uniqueTriangleNormals;
	if(mesh.HasTrianglesExtraDataContainer() && mesh.m_TrianglesExtraDataContainer[0].Has<TriangleNormalExtraData>())
	{
		std::unordered_set<Vec3> triangleNormals;
		for(auto&& curTriangleED : mesh.m_TrianglesExtraDataContainer)
		{
			auto triangleNormalED = curTriangleED.Get<TriangleNormalExtraData>();
			assert(triangleNormalED != nullptr && "All vertices must have a triangle normal if one has it");
			const Vec3& triangleNormal = triangleNormalED->GetData();
			triangleNormals.insert(triangleNormal);
		}

		uniqueTriangleNormals.insert(uniqueTriangleNormals.begin(), triangleNormals.begin(), triangleNormals.end());
		for(auto&& curTriangleNormal : uniqueTriangleNormals)
		{
			file << "vn" << ' ' << curTriangleNormal.x << ' ' << curTriangleNormal.y << ' ' << curTriangleNormal.z
				 << '\n';
		}
	}

	bool hasNormals = !uniqueTriangleNormals.empty();

	for(TriangleIndex iTriangle = 0; iTriangle < mesh.GetTriangleCount(); ++iTriangle)
	{
		const Triangle& curTriangle = mesh.m_Triangles[iTriangle];
		auto&& curContainer = mesh.m_TrianglesExtraDataContainer[iTriangle];
		file << 'f';
		for(auto curVertexIdx : curTriangle.Vertices)
		{
			// OBJ format uses 1-based indexing
			file << ' ' << (curVertexIdx + 1);

			// Check if texture coordinates and normals are available
			if(hasTexCoords || hasNormals)
			{
				file << '/';
				if(hasTexCoords)
				{
					auto texCoordsED = curContainer.Get<VerticesTexCoordsExtraData>();
					assert(texCoordsED != nullptr);
					int curVertexLocalIdx = GetVertexLocalIndex(curTriangle, curVertexIdx);
					assert(curVertexLocalIdx != -1);
					const Vec2& curTexCoods = texCoordsED->GetVertexTexCoords(curVertexLocalIdx);
					ptrdiff_t position = std::find_if(
											 std::begin(uniqueTexCoords),
											 std::end(uniqueTexCoords),
											 [curTexCoods](const Vec2& rhs)
											 {
												 return curTexCoods == rhs;
											 })
						- std::begin(uniqueTexCoords);
					assert(static_cast<size_t>(position) < uniqueTexCoords.size());
					file << static_cast<int>(position + 1);
				}

				if(hasNormals)
				{
					auto triangleNormalED = curContainer.Get<TriangleNormalExtraData>();
					assert(triangleNormalED != nullptr);
					const Vec3& triangleNormal = triangleNormalED->GetData();
					file << '/';
					ptrdiff_t position = std::find_if(
											 std::begin(uniqueTriangleNormals),
											 std::end(uniqueTriangleNormals),
											 [triangleNormal](const Vec3& rhs)
											 {
												 return triangleNormal == rhs;
											 })
						- std::begin(uniqueTriangleNormals);
					assert(static_cast<size_t>(position) < uniqueTriangleNormals.size());
					file << static_cast<int>(position + 1);
				}
			}
		}
		file << '\n';
	}

	file.close();
}
} // namespace Utilitary::Surface

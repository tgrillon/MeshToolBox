#include "Application/MeshBuffer.h"

#include "Application/ExtraDataTypes.h"
#include "Application/MeshLoader.h"
#include "Application/PrimitiveProxy.h"
#include "Core/PrintHelpers.h"

#include <string_view>

using namespace Data::Surface;
using namespace Data::Primitive;
using namespace Data::ExtraData;
using namespace Core::BaseType;
using namespace Utilitary::Surface;

namespace App
{

MeshBuffer::MeshBuffer(const std::filesystem::path& filepath, bool computeNormals)
{
	// Call the appropriate loading function.
	std::string_view extension = filepath.extension().string();
	std::unique_ptr<Mesh> mesh;
	if(extension == ".obj")
	{
		mesh = MeshLoader::LoadOBJ(filepath);
	}
	else if(extension == ".off")
	{
		mesh = MeshLoader::LoadOFF(filepath);
	}

	// If a mesh has been loaded.
	if(mesh)
	{
		if(computeNormals)
			mesh->ComputeTriangleNormals();

		// Reserve the appropriate amount of space in memory.
		const uint32_t& triangleCount = mesh->GetTriangleCount();
		m_Data.VertexPositions.reserve(triangleCount * 3);
		if(mesh->GetTriangle(0).HasExtraData<VerticesTexCoordsExtraData>())
		{
			m_Data.VertexTexCoords.reserve(triangleCount * 3);
		}
		if(mesh->GetTriangle(0).HasExtraData<TriangleNormalExtraData>())
		{
			m_Data.VertexNormals.reserve(triangleCount * 3);
		}

		// For each triangle of the loaded mesh.
		for(TriangleIndex iTriangle = 0; iTriangle < triangleCount; ++iTriangle)
		{
			TriangleProxy curTriangle = mesh->GetTriangle(iTriangle);

			// Store the position of each vertex.
			for(VertexLocalIndex iVertex = 0; iVertex == 3; ++iVertex)
			{
				VertexIndex curVertexIdx = curTriangle.GetVertex(iVertex);
				m_Data.VertexPositions.emplace_back(mesh->GetVertexData(curVertexIdx).Position);
			}

			// Store the texture coordinates of each vertex.
			if(curTriangle.HasExtraData<VerticesTexCoordsExtraData>())
			{
				for(VertexLocalIndex iVertex = 0; iVertex == 3; ++iVertex)
				{
					auto verticesTexCoords = curTriangle.GetExtraData<VerticesTexCoordsExtraData>();
					m_Data.VertexTexCoords.emplace_back(verticesTexCoords->GetVertexTexCoords(iVertex));
				}
			}

			// Store the flat normal of each vertex.
			if(curTriangle.HasExtraData<TriangleNormalExtraData>())
			{
				Vec3 curTriangleNormal = curTriangle.GetExtraData<TriangleNormalExtraData>()->GetData();
				for(VertexLocalIndex iVertex = 0; iVertex == 3; ++iVertex)
				{
					m_Data.VertexNormals.emplace_back(curTriangleNormal);
				}
			}
		}
	}
	else
	{
		Debug("No mesh found in : {}", filepath.string());
	}
}
} // namespace App

#include "Application/MeshLoader.h"

#include "Application/ExtraDataTypes.h"
#include "Application/PrimitiveProxy.h"
#include "Application/VertexPair.h"
#include "Core/PrintHelpers.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <utility>

using namespace Data::Surface;
using namespace Data::Primitive;
using namespace Data::ExtraData;
using namespace Core::BaseType;

namespace
{
/// @brief Helper function to skip comments and empty lines.
void SkipCommentsAndWhitespace(std::ifstream& file)
{
	while(file.peek() == '#' || file.peek() == '\n' || file.peek() == '\r' || file.peek() == ' ')
	{
		if(file.peek() == '#')
		{
			// Skip the entire comment line
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else
		{
			// Skip whitespace
			file.get();
		}
	}
}

/// @brief Helper function to read the next integer within a file.
int ReadNextInteger(std::ifstream& file)
{
	char buffer[16];
	file >> std::ws; // Skip leading whitespace

	// Read until first '/' or whitespace
	int i = 0;
	while(file.peek() != EOF && !std::isspace(file.peek()) && file.peek() != '/')
	{
		buffer[i++] = file.get();
	}
	buffer[i] = '\0';
	if(buffer[0] == '\0')
		return -1;

	// Convert string to index
	return std::stoi(buffer) - 1;
}
} // namespace

namespace Utilitary::Surface
{
std::unique_ptr<Mesh> MeshLoader::LoadOFF(const std::filesystem::path& filepath)
{
	std::ifstream file(filepath);

	// Checking file opening
	if(!file.is_open())
	{
		Error("Failed to open file: {}", filepath.string());
		return nullptr;
	}

	// Checking file type
	SkipCommentsAndWhitespace(file);
	std::string type;
	file >> type;
	if(type != "OFF")
	{
		Error("Wrong file format (must be OFF) : {}", filepath.string());
		return nullptr;
	}

	auto mesh = std::make_unique<Mesh>();

	// Retrieving the number of vertices / faces
	SkipCommentsAndWhitespace(file);
	int vertexCount, faceCount, unusedEdgeCount;
	file >> vertexCount >> faceCount >> unusedEdgeCount;

	// Reading vertices
	mesh->m_Vertices.resize(vertexCount);
	for(auto&& curVertex : mesh->m_Vertices)
	{
		SkipCommentsAndWhitespace(file);
		file >> curVertex.Position.x >> curVertex.Position.y >> curVertex.Position.z;
	}

	// Map to store edges and their corresponding triangle and edge index
	std::unordered_map<VertexPair, std::pair<TriangleIndex, EdgeIndex>> neighborMap;

	// Set neighboring faces using the edges
	auto SetFacesNeigbhor =
		[&](TriangleIndex faceIdx, VertexIndex firstVertexIdx, VertexIndex secondVertexIdx, EdgeIndex edgeIdx)
	{
		if(neighborMap.find({ firstVertexIdx, secondVertexIdx }) == neighborMap.end())
		{
			neighborMap[{ firstVertexIdx, secondVertexIdx }] = { faceIdx, edgeIdx };
		}
		else // The edge with firstVertex and secondVertex is already registered in map
		{
			auto [faceNeighborIdx, neighborEdgeIdx] = neighborMap[{ firstVertexIdx, secondVertexIdx }];
			mesh->m_Triangles[faceNeighborIdx].Neighbors[neighborEdgeIdx] = faceIdx;
			mesh->m_Triangles[faceIdx].Neighbors[edgeIdx] = faceNeighborIdx;
		}
	};

	mesh->m_Triangles.resize(faceCount);
	for(int iTriangle = 0; iTriangle < faceCount; ++iTriangle)
	{
		Triangle& curFace = mesh->m_Triangles[iTriangle];

		SkipCommentsAndWhitespace(file);

		// Set the triangle index
		file >> vertexCount;

		for(int iEdge = 0; iEdge < vertexCount; ++iEdge)
		{
			VertexIndex curVertexIdx;
			file >> curVertexIdx;

			// Get pointers to the vertex.
			Vertex& curVertex = mesh->m_Vertices[curVertexIdx];

			// Set IncidentTriangleIdx for the vertex if it's not already the case.
			if(curVertex.IncidentTriangleIdx == -1)
				curVertex.IncidentTriangleIdx = iTriangle;

			// Set vertices of the triangle.
			curFace.Vertices[iEdge] = curVertexIdx;
		}

		const VertexIndex iVertex0 = static_cast<VertexIndex>(curFace.Vertices[0]);
		const VertexIndex iVertex1 = static_cast<VertexIndex>(curFace.Vertices[1]);
		const VertexIndex iVertex2 = static_cast<VertexIndex>(curFace.Vertices[2]);

		// Edge v0-v1
		SetFacesNeigbhor(iTriangle, iVertex0, iVertex1, 2);
		// Edge v1-v2
		SetFacesNeigbhor(iTriangle, iVertex1, iVertex2, 0);
		// Edge v2-v0
		SetFacesNeigbhor(iTriangle, iVertex2, iVertex0, 1);
	}

	file.close();
	return mesh;
}

std::unique_ptr<Mesh> MeshLoader::LoadOBJ(const std::filesystem::path& filepath)
{
	std::ifstream file(filepath);

	// Checking file opening
	if(!file.is_open())
	{
		Error("Failed to open file: {}", filepath.string());
		return nullptr;
	}

	// Check file extension
	if(filepath.extension() != ".obj")
	{
		Error("Wrong file extension (must be .obj): {}", filepath.string());
		return nullptr;
	}

	auto mesh = std::make_unique<Mesh>();

	// Map to store edges and their corresponding triangle and edge index.
	std::unordered_map<VertexPair, std::pair<TriangleIndex, EdgeIndex>> neighborMap;

	// While store texture coordinates informations.
	std::vector<Vec2> texCoords;
	// While store triangle (flat) normal informations.
	std::vector<Vec3> flatNormals;

	// Set neighboring faces using the edges
	auto SetFacesNeigbhor =
		[&](TriangleIndex faceIdx, VertexIndex firstVertexIdx, VertexIndex secondVertexIdx, EdgeIndex edgeIdx)
	{
		if(neighborMap.find({ firstVertexIdx, secondVertexIdx }) == neighborMap.end())
		{
			neighborMap[{ firstVertexIdx, secondVertexIdx }] = { faceIdx, edgeIdx };
		}
		else // The edge with firstVertex and secondVertex is already registered in map
		{
			auto [faceNeighborIdx, neighborEdgeIdx] = neighborMap[{ firstVertexIdx, secondVertexIdx }];
			mesh->m_Triangles[faceNeighborIdx].Neighbors[neighborEdgeIdx] = faceIdx;
			mesh->m_Triangles[faceIdx].Neighbors[edgeIdx] = faceNeighborIdx;
		}
	};

	std::string type;
	while(file.peek() != EOF)
	{
		SkipCommentsAndWhitespace(file);

		if(file.peek() == EOF)
			break;

		file >> type;
		// Process line based on its type
		if(type == "mtllib") // Ignore this for now.
		{
			std::string ignoreMtlFile;
			std::getline(file, ignoreMtlFile);
		}
		else if(type == "g")
		{
			std::string objName;
			file >> objName;
			Info("Loading {} object from {} file...", objName, filepath.string());

			SkipCommentsAndWhitespace(file);
		}
		else if(type == "v")
		{ // Vertex position
			Vertex curVertex;
			file >> curVertex.Position.x >> curVertex.Position.y >> curVertex.Position.z;
			mesh->m_Vertices.emplace_back(curVertex);

			SkipCommentsAndWhitespace(file);
		}
		else if(type == "vt")
		{ // Vertex texture coordinate
			Vec2 vertexTexCoords;
			file >> vertexTexCoords.x >> vertexTexCoords.y;
			texCoords.emplace_back(vertexTexCoords);

			SkipCommentsAndWhitespace(file);
		}
		else if(type == "vn")
		{ // Normal vector
			Vec3 normal;
			file >> normal.x >> normal.y >> normal.z;
			flatNormals.emplace_back(normal);

			SkipCommentsAndWhitespace(file);
		}
		else if(type == "usemtl") // Ignore this for now.
		{
			std::string ignoreMaterialName;
			std::getline(file, ignoreMaterialName);
		}
		else if(type == "f")
		{ // Triangle (triangle)
			const TriangleIndex curTriangleIdx = mesh->GetTriangleCount();
			Triangle& curFace = mesh->m_Triangles.emplace_back();
			auto& curContainer = mesh->m_TrianglesExtraDataContainer.emplace_back();

			// Read three vertices for the triangle
			for(VertexLocalIndex iVertex = 0; iVertex < 3; ++iVertex)
			{
				int curVertexIdx = ReadNextInteger(file);
				assert(curVertexIdx != -1);
				Vertex& curVertex = mesh->m_Vertices[curVertexIdx];
				if(curVertex.IncidentTriangleIdx == -1)
					curVertex.IncidentTriangleIdx = curTriangleIdx;

				curFace.Vertices[iVertex] = curVertexIdx;

				// Skip '/' character.
				file.ignore(1);

				int texCoordsIdx = ReadNextInteger(file);
				if(texCoordsIdx != -1)
				{ // Vertex texCoords index.
					auto& verticesTexCoords = curContainer.GetOrCreate<VerticesTexCoordsExtraData>();
					verticesTexCoords.SetVertexTexCoords(texCoords[texCoordsIdx], iVertex);
				}

				if(file.peek() != '/')
					continue;

				// Skip '/' character.
				file.ignore(1);

				int flatNormalIdx = ReadNextInteger(file);
				if(flatNormalIdx != -1)
				{ // Flat normal index.
					auto& faceNormal = curContainer.GetOrCreate<TriangleNormalExtraData>();
					faceNormal.SetData(flatNormals[flatNormalIdx]);
				}
			}

			const VertexIndex iVertex0 = static_cast<VertexIndex>(curFace.Vertices[0]);
			const VertexIndex iVertex1 = static_cast<VertexIndex>(curFace.Vertices[1]);
			const VertexIndex iVertex2 = static_cast<VertexIndex>(curFace.Vertices[2]);

			// Edge v0-v1
			SetFacesNeigbhor(curTriangleIdx, iVertex0, iVertex1, 2);
			// Edge v1-v2
			SetFacesNeigbhor(curTriangleIdx, iVertex1, iVertex2, 0);
			// Edge v2-v0
			SetFacesNeigbhor(curTriangleIdx, iVertex2, iVertex0, 1);
		}
	}

	file.close();
	return mesh;
}

} // namespace Utilitary::Surface

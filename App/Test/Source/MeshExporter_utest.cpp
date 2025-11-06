#include "Application/ExtraDataTypes.h"
#include "Application/MeshExporter.h"
#include "Application/PrimitiveProxy.h"
#include "Application/TestHelpers.h"
#include "Core/MathHelpers.h"
#include "Core/PrintHelpers.h"

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string_view>

using namespace Core::Math;
using namespace Utilitary::Surface;
using namespace Data::Surface;
using namespace Data::Primitive;
using namespace Data::ExtraData;
using namespace Core::BaseType;

TEST(MeshExporterTest, ValidMesh_ExportOFFShouldSucceed)
{
	// Create a simple valid mesh and export it.
	Mesh mesh = TestHelpers::CreateValidMesh();
	const std::filesystem::path filepath = std::filesystem::relative("TestFiles/Off/validMesh.off");
	MeshExporter::ExportOFF(mesh, filepath);

	// Read back the file and verify its contents.
	std::ifstream file(filepath);
	Debug("Reading from {}", filepath.string());
	EXPECT_TRUE(file.is_open());

	std::string type;
	file >> type;
	EXPECT_EQ(type, "OFF");

	uint32_t vertexCount, faceCount, edgeCount;
	file >> vertexCount >> faceCount >> edgeCount;
	EXPECT_EQ(vertexCount, mesh.GetVertexCount());
	EXPECT_EQ(faceCount, mesh.GetTriangleCount());
	EXPECT_EQ(edgeCount, 0);

	// Read and verify vertex positions.
	for(auto&& curVertex : mesh.GetVertices())
	{
		Vec3 position;
		file >> position.x >> position.y >> position.z;
		EXPECT_EQ(curVertex.Position, position);
	}

	// Read and verify triangle definitions.
	for(auto&& curFace : mesh.GetTriangles())
	{
		file >> vertexCount;
		for(int curVertexIdx : curFace.Vertices)
		{
			int vertexIdx;
			file >> vertexIdx;
			EXPECT_EQ(curVertexIdx + 1, vertexIdx);
		}
	}

	file.close();
}

TEST(MeshExporterTest, ValidMesh_ExportOBJShouldSucceed)
{
	// Create a simple valid mesh and export it.
	Mesh mesh = TestHelpers::CreateValidMesh();
	const std::filesystem::path filepath = std::filesystem::relative("TestFiles/Obj/validMesh.obj");
	MeshExporter::ExportOBJ(mesh, filepath);

	// Read back the file and verify its contents.
	std::ifstream file(filepath);
	Debug("Reading from {}", filepath.string());
	EXPECT_TRUE(file.is_open());

	// Read and verify vertex positions.
	for(auto&& curVertex : mesh.GetVertices())
	{
		char vChar;
		file >> vChar;
		EXPECT_EQ(vChar, 'v');

		Vec3 position;
		file >> position.x >> position.y >> position.z;
		EXPECT_EQ(curVertex.Position, position);
	}

	// Read and verify triangle definitions.
	for(auto&& curFace : mesh.GetTriangles())
	{
		char fChar;
		file >> fChar;
		EXPECT_EQ(fChar, 'f');

		for(auto curVertexIdx : curFace.Vertices)
		{
			int vertexIdx;
			file >> vertexIdx;
			EXPECT_EQ(curVertexIdx + 1, vertexIdx);
		}
	}

	file.close();
}

TEST(MeshExporterTest, ValidMesh_ExportOBJWithEDShouldSucceed)
{
	// Create a simple valid mesh and export it.
	Mesh mesh = TestHelpers::CreateValidMeshWithED();
	const std::filesystem::path filepath = std::filesystem::relative("TestFiles/Obj/validMeshWithED.obj");
	MeshExporter::ExportOBJ(mesh, filepath);

	std::string expectedFileContent =
		"v 0 0 0\nv 1 0 0\nv 1 1 0\nv 0 1 0\nvt 0 1\nvt 1 0\nvt 1 1\nvt 0 0\nvn 1 0 0\nf 1/4/1 2/2/1 3/3/1\nf 1/4/1 "
		"3/3/1 4/1/1\n";

	std::ifstream t(filepath);
	std::stringstream buffer;
	buffer << t.rdbuf();

	EXPECT_EQ(buffer.str(), expectedFileContent);
}

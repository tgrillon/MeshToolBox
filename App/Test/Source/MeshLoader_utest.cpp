#include "Application/ExtraDataTypes.h"
#include "Application/MeshIntegrity.h"
#include "Application/MeshLoader.h"
#include "Application/PrimitiveProxy.h"
#include "Core/MathHelpers.h"

#include <gtest/gtest.h>

using namespace Utilitary::Surface;
using namespace Data::ExtraData;
using namespace Data::Primitive;
using namespace Data::Surface;
using namespace Core::Math::Compare;
using namespace Core::BaseType;

TEST(MeshLoaderTest, LoadOFF_ValidFile_ShouldLoadMesh)
{
	std::unique_ptr<Data::Surface::Mesh> mesh = MeshLoader::LoadOFF("TestFiles/Off/cube.off");
	ASSERT_NE(mesh, nullptr);

	EXPECT_EQ(mesh->GetVertexCount(), 8);
	EXPECT_EQ(mesh->GetTriangleCount(), 12);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(*mesh), MeshIntegrity::ExitCode::MeshOK);
}

TEST(MeshLoaderTest, LoadOFF_InvalidFile_ShouldReturnNullptr)
{
	{ // Wrong file extension.
		std::unique_ptr<Data::Surface::Mesh> mesh = MeshLoader::LoadOFF("TestFiles/Obj/cube.obj");
		EXPECT_EQ(mesh, nullptr);
	}

	{ // Can't open file.
		std::unique_ptr<Data::Surface::Mesh> mesh = MeshLoader::LoadOFF("TestFiles/Off/notAFile.off");
		EXPECT_EQ(mesh, nullptr);
	}
}

TEST(MeshLoaderTest, LoadOBJ_ValidFile_ShouldLoadMesh)
{
	std::unique_ptr<Data::Surface::Mesh> mesh = MeshLoader::LoadOBJ("TestFiles/Obj/cube.obj");
	ASSERT_NE(mesh, nullptr);
	EXPECT_EQ(mesh->GetVertexCount(), 8); // 8 vertices (a-h)
	EXPECT_EQ(mesh->GetTriangleCount(), 12); // 12 triangles (6 faces × 2 triangles)
	EXPECT_EQ(MeshIntegrity::CheckIntegrity(*mesh), MeshIntegrity::ExitCode::MeshOK);
}

TEST(MeshLoaderTest, LoadOBJ_InvalidFile_ShouldReturnNullptr)
{
	{ // Wrong file extension.
		std::unique_ptr<Data::Surface::Mesh> mesh = MeshLoader::LoadOBJ("TestFiles/Off/cube.off");
		EXPECT_EQ(mesh, nullptr);
	}
	{ // Can't open file.
		std::unique_ptr<Data::Surface::Mesh> mesh = MeshLoader::LoadOBJ("TestFiles/Obj/notAFile.obj");
		EXPECT_EQ(mesh, nullptr);
	}
}

TEST(MeshLoaderTest, LoadOBJ_OBJWithVtAndVn_ShouldBeCorrect)
{
	std::unique_ptr<Data::Surface::Mesh> mesh = MeshLoader::LoadOBJ("TestFiles/Obj/cube_vtvn.obj");
	ASSERT_NE(mesh, nullptr);

	// Check vertices.
	ASSERT_EQ(mesh->GetVertexCount(), 8);

	// Check positions of the 8 vertices of the cube
	EXPECT_TRUE(EqualNear(mesh->GetVertexData(0).Position, Vec3{ 0.0f, 0.0f, 0.0f })); // v 0.0 0.0 0.0  # 1 a
	EXPECT_TRUE(EqualNear(mesh->GetVertexData(1).Position, Vec3{ 0.0f, 1.0f, 0.0f })); // v 0.0 1.0 0.0  # 2 b
	EXPECT_TRUE(EqualNear(mesh->GetVertexData(2).Position, Vec3{ 1.0f, 1.0f, 0.0f })); // v 1.0 1.0 0.0  # 3 c
	EXPECT_TRUE(EqualNear(mesh->GetVertexData(3).Position, Vec3{ 1.0f, 0.0f, 0.0f })); // v 1.0 0.0 0.0  # 4 d
	EXPECT_TRUE(EqualNear(mesh->GetVertexData(4).Position, Vec3{ 0.0f, 0.0f, 1.0f })); // v 0.0 0.0 1.0  # 5 e
	EXPECT_TRUE(EqualNear(mesh->GetVertexData(5).Position, Vec3{ 0.0f, 1.0f, 1.0f })); // v 0.0 1.0 1.0  # 6 f
	EXPECT_TRUE(EqualNear(mesh->GetVertexData(6).Position, Vec3{ 1.0f, 1.0f, 1.0f })); // v 1.0 1.0 1.0  # 7 g
	EXPECT_TRUE(EqualNear(mesh->GetVertexData(7).Position, Vec3{ 1.0f, 0.0f, 1.0f })); // v 1.0 0.0 1.0  # 8 h

	// Check triangles.
	ASSERT_EQ(mesh->GetTriangleCount(), 12);

	// Check first triangle of face 1: cgh (vertices 3,7,8 -> indices 2,6,7)
	// f 3/10/1 7/6/1 8/5/1
	EXPECT_EQ(mesh->GetTriangleData(0).Vertices[0], 2); // vertex c (index 2)
	EXPECT_EQ(mesh->GetTriangleData(0).Vertices[1], 6); // vertex g (index 6)
	EXPECT_EQ(mesh->GetTriangleData(0).Vertices[2], 7); // vertex h (index 7)

	// Check second triangle of face 1: chd (vertices 3,8,4 -> indices 2,7,3)
	// f 3/10/1 8/5/1 4/9/1
	EXPECT_EQ(mesh->GetTriangleData(1).Vertices[0], 2); // vertex c (index 2)
	EXPECT_EQ(mesh->GetTriangleData(1).Vertices[1], 7); // vertex h (index 7)
	EXPECT_EQ(mesh->GetTriangleData(1).Vertices[2], 3); // vertex d (index 3)

	// Check second triangle of face 2: aef (vertices 1,5,6 -> indices 0,4,5)
	// f 1/8/2 5/4/2 6/3/2
	EXPECT_EQ(mesh->GetTriangleData(2).Vertices[0], 0); // vertex a (index 0)
	EXPECT_EQ(mesh->GetTriangleData(2).Vertices[1], 4); // vertex e (index 4)
	EXPECT_EQ(mesh->GetTriangleData(2).Vertices[2], 5); // vertex f (index 5)

	// Check second triangle of face 2: afb (vertices 1,6,2 -> indices 0,5,1)
	// f 1/8/2 6/3/2 2/7/2
	EXPECT_EQ(mesh->GetTriangleData(3).Vertices[0], 0); // vertex a (index 0)
	EXPECT_EQ(mesh->GetTriangleData(3).Vertices[1], 5); // vertex f (index 5)
	EXPECT_EQ(mesh->GetTriangleData(3).Vertices[2], 1); // vertex b (index 1)

	{ // Check vertices texture coordinates of the first triangle of face 1: cgh (vertices 3,7,8 -> indices 2,6,7)
		// f 3/10/1 7/6/1 8/5/1
		const TriangleProxy& triangle = mesh->GetTriangle(0);
		auto verticesTexCoords = triangle.GetExtraData<VerticesTexCoordsExtraData>();
		ASSERT_NE(verticesTexCoords, nullptr);
		EXPECT_TRUE(EqualNear(verticesTexCoords->GetVertexTexCoords(0), Vec2{ 0.75, 0.50 }));
		EXPECT_TRUE(EqualNear(verticesTexCoords->GetVertexTexCoords(1), Vec2{ 0.75, 0.75 }));
		EXPECT_TRUE(EqualNear(verticesTexCoords->GetVertexTexCoords(2), Vec2{ 0.50, 0.75 }));
	}

	{ // Check vertices texture coordinates of the second triangle of face 1: chd (vertices 3,8,4 -> indices 2,7,3)
		// f 3/10/1 8/5/1 4/9/1
		const TriangleProxy& triangle = mesh->GetTriangle(1);
		auto verticesTexCoords = triangle.GetExtraData<VerticesTexCoordsExtraData>();
		ASSERT_NE(verticesTexCoords, nullptr);
		EXPECT_TRUE(EqualNear(verticesTexCoords->GetVertexTexCoords(0), Vec2{ 0.75, 0.50 }));
		EXPECT_TRUE(EqualNear(verticesTexCoords->GetVertexTexCoords(1), Vec2{ 0.50, 0.75 }));
		EXPECT_TRUE(EqualNear(verticesTexCoords->GetVertexTexCoords(2), Vec2{ 0.50, 0.50 }));
	}

	{ // Check vertices texture coordinates of the first triangle of face 2: aef (vertices 1,5,6 -> indices 0,4,5)
		// f 1/8/2 5/4/2 6/3/2
		const TriangleProxy& triangle = mesh->GetTriangle(2);
		auto verticesTexCoords = triangle.GetExtraData<VerticesTexCoordsExtraData>();
		ASSERT_NE(verticesTexCoords, nullptr);
		EXPECT_TRUE(EqualNear(verticesTexCoords->GetVertexTexCoords(0), Vec2{ 0.25, 0.50 }));
		EXPECT_TRUE(EqualNear(verticesTexCoords->GetVertexTexCoords(1), Vec2{ 0.25, 0.75 }));
		EXPECT_TRUE(EqualNear(verticesTexCoords->GetVertexTexCoords(2), Vec2{ 0.0, 0.75 }));
	}

	{ // Check vertices texture coordinates of the second triangle of face 2: afb (vertices 1,6,2 -> indices 0,5,1)
		// f 1/8/2 6/3/2 2/7/2
		const TriangleProxy& triangle = mesh->GetTriangle(3);
		auto verticesTexCoords = triangle.GetExtraData<VerticesTexCoordsExtraData>();
		ASSERT_NE(verticesTexCoords, nullptr);
		EXPECT_TRUE(EqualNear(verticesTexCoords->GetVertexTexCoords(0), Vec2{ 0.25, 0.50 }));
		EXPECT_TRUE(EqualNear(verticesTexCoords->GetVertexTexCoords(1), Vec2{ 0.0, 0.75 }));
		EXPECT_TRUE(EqualNear(verticesTexCoords->GetVertexTexCoords(2), Vec2{ 0.0, 0.50 }));
	}

	{ // Check vertices triangle normal of the first triangle of face 1: cgh (vertices 3,7,8 -> indices 2,6,7)
		// f 3/10/1 7/6/1 8/5/1
		const TriangleProxy& triangle = mesh->GetTriangle(0);
		auto triangleNormal = triangle.GetExtraData<TriangleNormalExtraData>();
		ASSERT_NE(triangleNormal, nullptr);
		EXPECT_TRUE(EqualNear(triangleNormal->GetData(), Vec3{ 1.0, 0.0, 0.0 }));
	}

	{ // Check vertices triangle normal of the second triangle of face 1: chd (vertices 3,8,4 -> indices 2,7,3)
		// f 3/10/1 8/5/1 4/9/1
		const TriangleProxy& triangle = mesh->GetTriangle(1);
		auto triangleNormal = triangle.GetExtraData<TriangleNormalExtraData>();
		ASSERT_NE(triangleNormal, nullptr);
		EXPECT_TRUE(EqualNear(triangleNormal->GetData(), Vec3{ 1.0, 0.0, 0.0 }));
	}

	{ // Check vertices triangle normal of the first triangle of face 2: aef (vertices 1,5,6 -> indices 0,4,5)
		// f 1/8/2 5/4/2 6/3/2
		const TriangleProxy& triangle = mesh->GetTriangle(2);
		auto triangleNormal = triangle.GetExtraData<TriangleNormalExtraData>();
		ASSERT_NE(triangleNormal, nullptr);
		EXPECT_TRUE(EqualNear(triangleNormal->GetData(), Vec3{ -1.0, 0.0, 0.0 }));
	}

	{ // Check vertices triangle normal of the second triangle of face 2: afb (vertices 1,6,2 -> indices 0,5,1)
		// f 1/8/2 6/3/2 2/7/2
		const TriangleProxy& triangle = mesh->GetTriangle(3);
		auto triangleNormal = triangle.GetExtraData<TriangleNormalExtraData>();
		ASSERT_NE(triangleNormal, nullptr);
		EXPECT_TRUE(EqualNear(triangleNormal->GetData(), Vec3{ -1.0, 0.0, 0.0 }));
	}
}

TEST(MeshLoaderTest, LoadOBJ_OBJWithoutVn_ShouldBeLoaded)
{
	std::unique_ptr<Mesh> mesh = MeshLoader::LoadOBJ("TestFiles/Obj/cube_vt.obj");
	ASSERT_NE(mesh, nullptr);

	// Check vertices.
	ASSERT_EQ(mesh->GetVertexCount(), 8);

	// Check positions of the 8 vertices of the cube
	EXPECT_TRUE(EqualNear(mesh->GetVertexData(0).Position, Vec3{ 0.0f, 0.0f, 0.0f })); // v 0.0 0.0 0.0  # 1 a
	EXPECT_TRUE(EqualNear(mesh->GetVertexData(1).Position, Vec3{ 0.0f, 1.0f, 0.0f })); // v 0.0 1.0 0.0  # 2 b
	EXPECT_TRUE(EqualNear(mesh->GetVertexData(2).Position, Vec3{ 1.0f, 1.0f, 0.0f })); // v 1.0 1.0 0.0  # 3 c
	EXPECT_TRUE(EqualNear(mesh->GetVertexData(3).Position, Vec3{ 1.0f, 0.0f, 0.0f })); // v 1.0 0.0 0.0  # 4 d
	EXPECT_TRUE(EqualNear(mesh->GetVertexData(4).Position, Vec3{ 0.0f, 0.0f, 1.0f })); // v 0.0 0.0 1.0  # 5 e
	EXPECT_TRUE(EqualNear(mesh->GetVertexData(5).Position, Vec3{ 0.0f, 1.0f, 1.0f })); // v 0.0 1.0 1.0  # 6 f
	EXPECT_TRUE(EqualNear(mesh->GetVertexData(6).Position, Vec3{ 1.0f, 1.0f, 1.0f })); // v 1.0 1.0 1.0  # 7 g
	EXPECT_TRUE(EqualNear(mesh->GetVertexData(7).Position, Vec3{ 1.0f, 0.0f, 1.0f })); // v 1.0 0.0 1.0  # 8 h

	// Check triangles.
	ASSERT_EQ(mesh->GetTriangleCount(), 12);

	// Check first triangle of face 1: cgh (vertices 3,7,8 -> indices 2,6,7)
	// f 3/10/1 7/6/1 8/5/1
	EXPECT_EQ(mesh->GetTriangleData(0).Vertices[0], 2); // vertex c (index 2)
	EXPECT_EQ(mesh->GetTriangleData(0).Vertices[1], 6); // vertex g (index 6)
	EXPECT_EQ(mesh->GetTriangleData(0).Vertices[2], 7); // vertex h (index 7)

	// Check second triangle of face 1: chd (vertices 3,8,4 -> indices 2,7,3)
	// f 3/10/1 8/5/1 4/9/1
	EXPECT_EQ(mesh->GetTriangleData(1).Vertices[0], 2); // vertex c (index 2)
	EXPECT_EQ(mesh->GetTriangleData(1).Vertices[1], 7); // vertex h (index 7)
	EXPECT_EQ(mesh->GetTriangleData(1).Vertices[2], 3); // vertex d (index 3)

	// Check second triangle of face 2: aef (vertices 1,5,6 -> indices 0,4,5)
	// f 1/8/2 5/4/2 6/3/2
	EXPECT_EQ(mesh->GetTriangleData(2).Vertices[0], 0); // vertex a (index 0)
	EXPECT_EQ(mesh->GetTriangleData(2).Vertices[1], 4); // vertex e (index 4)
	EXPECT_EQ(mesh->GetTriangleData(2).Vertices[2], 5); // vertex f (index 5)

	// Check second triangle of face 2: afb (vertices 1,6,2 -> indices 0,5,1)
	// f 1/8/2 6/3/2 2/7/2
	EXPECT_EQ(mesh->GetTriangleData(3).Vertices[0], 0); // vertex a (index 0)
	EXPECT_EQ(mesh->GetTriangleData(3).Vertices[1], 5); // vertex f (index 5)
	EXPECT_EQ(mesh->GetTriangleData(3).Vertices[2], 1); // vertex b (index 1)

	{ // Check vertices texture coordinates of the first triangle of face 1: cgh (vertices 3,7,8 -> indices 2,6,7)
		// f 3/10/1 7/6/1 8/5/1
		const TriangleProxy& triangle = mesh->GetTriangle(0);
		auto verticesTexCoords = triangle.GetExtraData<VerticesTexCoordsExtraData>();
		ASSERT_NE(verticesTexCoords, nullptr);
		EXPECT_TRUE(EqualNear(verticesTexCoords->GetVertexTexCoords(0), Vec2{ 0.75, 0.50 }));
		EXPECT_TRUE(EqualNear(verticesTexCoords->GetVertexTexCoords(1), Vec2{ 0.75, 0.75 }));
		EXPECT_TRUE(EqualNear(verticesTexCoords->GetVertexTexCoords(2), Vec2{ 0.50, 0.75 }));
	}

	{ // Check vertices texture coordinates of the second triangle of face 1: chd (vertices 3,8,4 -> indices 2,7,3)
		// f 3/10/1 8/5/1 4/9/1
		const TriangleProxy& triangle = mesh->GetTriangle(1);
		auto verticesTexCoords = triangle.GetExtraData<VerticesTexCoordsExtraData>();
		ASSERT_NE(verticesTexCoords, nullptr);
		EXPECT_TRUE(EqualNear(verticesTexCoords->GetVertexTexCoords(0), Vec2{ 0.75, 0.50 }));
		EXPECT_TRUE(EqualNear(verticesTexCoords->GetVertexTexCoords(1), Vec2{ 0.50, 0.75 }));
		EXPECT_TRUE(EqualNear(verticesTexCoords->GetVertexTexCoords(2), Vec2{ 0.50, 0.50 }));
	}

	{ // Check vertices texture coordinates of the first triangle of face 2: aef (vertices 1,5,6 -> indices 0,4,5)
		// f 1/8/2 5/4/2 6/3/2
		const TriangleProxy& triangle = mesh->GetTriangle(2);
		auto verticesTexCoords = triangle.GetExtraData<VerticesTexCoordsExtraData>();
		ASSERT_NE(verticesTexCoords, nullptr);
		EXPECT_TRUE(EqualNear(verticesTexCoords->GetVertexTexCoords(0), Vec2{ 0.25, 0.50 }));
		EXPECT_TRUE(EqualNear(verticesTexCoords->GetVertexTexCoords(1), Vec2{ 0.25, 0.75 }));
		EXPECT_TRUE(EqualNear(verticesTexCoords->GetVertexTexCoords(2), Vec2{ 0.0, 0.75 }));
	}

	{ // Check vertices texture coordinates of the second triangle of face 2: afb (vertices 1,6,2 -> indices 0,5,1)
		// f 1/8/2 6/3/2 2/7/2
		const TriangleProxy& triangle = mesh->GetTriangle(3);
		auto verticesTexCoords = triangle.GetExtraData<VerticesTexCoordsExtraData>();
		ASSERT_NE(verticesTexCoords, nullptr);
		EXPECT_TRUE(EqualNear(verticesTexCoords->GetVertexTexCoords(0), Vec2{ 0.25, 0.50 }));
		EXPECT_TRUE(EqualNear(verticesTexCoords->GetVertexTexCoords(1), Vec2{ 0.0, 0.75 }));
		EXPECT_TRUE(EqualNear(verticesTexCoords->GetVertexTexCoords(2), Vec2{ 0.0, 0.50 }));
	}

	// Triangles should not contain an extra data with their normal.
	for(TriangleIndex iTriangle = 0; iTriangle < mesh->GetTriangleCount(); ++iTriangle)
	{
		const TriangleProxy& curTriangle = mesh->GetTriangle(iTriangle);
		EXPECT_FALSE(curTriangle.HasExtraData<TriangleNormalExtraData>());
	}
}

TEST(MeshLoaderTest, LoadOBJ_OBJWithoutVt_ShouldBeLoaded)
{
	std::unique_ptr<Mesh> mesh = MeshLoader::LoadOBJ("TestFiles/Obj/cube_vn.obj");
	ASSERT_NE(mesh, nullptr);

	// Check vertices.
	ASSERT_EQ(mesh->GetVertexCount(), 8);

	// Check positions of the 8 vertices of the cube
	EXPECT_TRUE(EqualNear(mesh->GetVertexData(0).Position, Vec3{ 0.0f, 0.0f, 0.0f })); // v 0.0 0.0 0.0  # 1 a
	EXPECT_TRUE(EqualNear(mesh->GetVertexData(1).Position, Vec3{ 0.0f, 1.0f, 0.0f })); // v 0.0 1.0 0.0  # 2 b
	EXPECT_TRUE(EqualNear(mesh->GetVertexData(2).Position, Vec3{ 1.0f, 1.0f, 0.0f })); // v 1.0 1.0 0.0  # 3 c
	EXPECT_TRUE(EqualNear(mesh->GetVertexData(3).Position, Vec3{ 1.0f, 0.0f, 0.0f })); // v 1.0 0.0 0.0  # 4 d
	EXPECT_TRUE(EqualNear(mesh->GetVertexData(4).Position, Vec3{ 0.0f, 0.0f, 1.0f })); // v 0.0 0.0 1.0  # 5 e
	EXPECT_TRUE(EqualNear(mesh->GetVertexData(5).Position, Vec3{ 0.0f, 1.0f, 1.0f })); // v 0.0 1.0 1.0  # 6 f
	EXPECT_TRUE(EqualNear(mesh->GetVertexData(6).Position, Vec3{ 1.0f, 1.0f, 1.0f })); // v 1.0 1.0 1.0  # 7 g
	EXPECT_TRUE(EqualNear(mesh->GetVertexData(7).Position, Vec3{ 1.0f, 0.0f, 1.0f })); // v 1.0 0.0 1.0  # 8 h

	// Check triangles.
	ASSERT_EQ(mesh->GetTriangleCount(), 12);

	// Check first triangle of face 1: cgh (vertices 3,7,8 -> indices 2,6,7)
	// f 3/10/1 7/6/1 8/5/1
	EXPECT_EQ(mesh->GetTriangleData(0).Vertices[0], 2); // vertex c (index 2)
	EXPECT_EQ(mesh->GetTriangleData(0).Vertices[1], 6); // vertex g (index 6)
	EXPECT_EQ(mesh->GetTriangleData(0).Vertices[2], 7); // vertex h (index 7)

	// Check second triangle of face 1: chd (vertices 3,8,4 -> indices 2,7,3)
	// f 3/10/1 8/5/1 4/9/1
	EXPECT_EQ(mesh->GetTriangleData(1).Vertices[0], 2); // vertex c (index 2)
	EXPECT_EQ(mesh->GetTriangleData(1).Vertices[1], 7); // vertex h (index 7)
	EXPECT_EQ(mesh->GetTriangleData(1).Vertices[2], 3); // vertex d (index 3)

	// Check second triangle of face 2: aef (vertices 1,5,6 -> indices 0,4,5)
	// f 1/8/2 5/4/2 6/3/2
	EXPECT_EQ(mesh->GetTriangleData(2).Vertices[0], 0); // vertex a (index 0)
	EXPECT_EQ(mesh->GetTriangleData(2).Vertices[1], 4); // vertex e (index 4)
	EXPECT_EQ(mesh->GetTriangleData(2).Vertices[2], 5); // vertex f (index 5)

	// Check second triangle of face 2: afb (vertices 1,6,2 -> indices 0,5,1)
	// f 1/8/2 6/3/2 2/7/2
	EXPECT_EQ(mesh->GetTriangleData(3).Vertices[0], 0); // vertex a (index 0)
	EXPECT_EQ(mesh->GetTriangleData(3).Vertices[1], 5); // vertex f (index 5)
	EXPECT_EQ(mesh->GetTriangleData(3).Vertices[2], 1); // vertex b (index 1)

	// Triangles should not contain an extra data with vertices texture coordinates.
	for(TriangleIndex iTriangle = 0; iTriangle < mesh->GetTriangleCount(); ++iTriangle)
	{
		const TriangleProxy& curTriangle = mesh->GetTriangle(iTriangle);
		EXPECT_FALSE(curTriangle.HasExtraData<VerticesTexCoordsExtraData>());
	}

	{ // Check vertices triangle normal of the first triangle of face 1: cgh (vertices 3,7,8 -> indices 2,6,7)
		// f 3/10/1 7/6/1 8/5/1
		const TriangleProxy& triangle = mesh->GetTriangle(0);
		auto triangleNormal = triangle.GetExtraData<TriangleNormalExtraData>();
		ASSERT_NE(triangleNormal, nullptr);
		EXPECT_TRUE(EqualNear(triangleNormal->GetData(), Vec3{ 1.0, 0.0, 0.0 }));
	}

	{ // Check vertices triangle normal of the second triangle of face 1: chd (vertices 3,8,4 -> indices 2,7,3)
		// f 3/10/1 8/5/1 4/9/1
		const TriangleProxy& triangle = mesh->GetTriangle(1);
		auto triangleNormal = triangle.GetExtraData<TriangleNormalExtraData>();
		ASSERT_NE(triangleNormal, nullptr);
		EXPECT_TRUE(EqualNear(triangleNormal->GetData(), Vec3{ 1.0, 0.0, 0.0 }));
	}

	{ // Check vertices triangle normal of the first triangle of face 2: aef (vertices 1,5,6 -> indices 0,4,5)
		// f 1/8/2 5/4/2 6/3/2
		const TriangleProxy& triangle = mesh->GetTriangle(2);
		auto triangleNormal = triangle.GetExtraData<TriangleNormalExtraData>();
		ASSERT_NE(triangleNormal, nullptr);
		EXPECT_TRUE(EqualNear(triangleNormal->GetData(), Vec3{ -1.0, 0.0, 0.0 }));
	}

	{ // Check vertices triangle normal of the second triangle of face 2: afb (vertices 1,6,2 -> indices 0,5,1)
		// f 1/8/2 6/3/2 2/7/2
		const TriangleProxy& triangle = mesh->GetTriangle(3);
		auto triangleNormal = triangle.GetExtraData<TriangleNormalExtraData>();
		ASSERT_NE(triangleNormal, nullptr);
		EXPECT_TRUE(EqualNear(triangleNormal->GetData(), Vec3{ -1.0, 0.0, 0.0 }));
	}
}

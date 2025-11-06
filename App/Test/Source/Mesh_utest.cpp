#include "Application/ExtraDataTypes.h"
#include "Application/Mesh.h"
#include "Application/MeshIntegrity.h"
#include "Application/MeshLoader.h"
#include "Application/PrimitiveProxy.h"
#include "Application/TestHelpers.h"
#include "Core/MathHelpers.h"

#include <gtest/gtest.h>

using namespace Core::BaseType;
using namespace Utilitary::Surface;
using namespace Data::Surface;
using namespace Data::ExtraData;
using namespace Data::Primitive;
using namespace Core::Math::Compare;

TEST(MeshTest, CopyConstructor_ShouldDeepCopyMesh)
{
	Mesh originalMesh = TestHelpers::CreateValidMesh();

	// Copy the mesh
	Mesh copiedMesh(originalMesh);

	// Verify that the copied mesh has the same number of vertices and faces
	EXPECT_EQ(copiedMesh.GetVertexCount(), originalMesh.GetVertexCount());
	EXPECT_EQ(copiedMesh.GetTriangleCount(), originalMesh.GetTriangleCount());

	// Modify the original mesh and verify that the copied mesh remains unchanged
	originalMesh.GetVertexData(0).Position = { 2., 2., 2. };
	EXPECT_NE(copiedMesh.GetVertexData(0).Position, originalMesh.GetVertexData(0).Position);

	originalMesh.GetTriangleData(0).Vertices[0] = 3;
	EXPECT_NE(copiedMesh.GetTriangleData(0).Vertices[0], originalMesh.GetTriangleData(0).Vertices[0]);
}

TEST(MeshTest, AddVertexAndFace_ShouldReturnCorrectIndices)
{
	Mesh mesh;

	// Add vertices
	VertexIndex vIdx1 = mesh.AddVertex({ .Position = { 0., 0., 0. }, .IncidentTriangleIdx = { -1 } });
	VertexIndex vIdx2 = mesh.AddVertex({ .Position = { 1., 1., 1. }, .IncidentTriangleIdx = { -1 } });

	// Verify vertex indices
	EXPECT_EQ(vIdx1, 0);
	EXPECT_EQ(vIdx2, 1);
	EXPECT_EQ(mesh.GetVertexCount(), 2);

	// Add a triangle
	TriangleIndex fIdx = mesh.AddTriangle(
		{ .Vertices = { static_cast<int>(vIdx1), static_cast<int>(vIdx2), -1 }, .Neighbors = { -1, -1, -1 } });

	// Verify triangle index
	EXPECT_EQ(fIdx, 0);
	EXPECT_EQ(mesh.GetTriangleCount(), 1);
}

TEST(MeshTest, GetVertexAndFace_ShouldReturnProxies)
{
	Mesh mesh = TestHelpers::CreateValidMesh();

	// Get vertex proxy
	VertexProxy vertexProxy = mesh.GetVertex(0);
	EXPECT_EQ(vertexProxy.GetIndex(), 0);
	EXPECT_EQ(vertexProxy.GetPosition(), (Vec3{ 0., 0., 0. }));

	// Get triangle proxy
	TriangleProxy faceProxy = mesh.GetTriangle(0);
	EXPECT_EQ(faceProxy.GetIndex(), 0);
	EXPECT_EQ(faceProxy.GetVertex(0), 0);
	EXPECT_EQ(faceProxy.GetVertex(1), 1);
	EXPECT_EQ(faceProxy.GetVertex(2), 2);
}

TEST(MeshTest, GetVertexAndFaceData_ShouldReturnReferences)
{
	Mesh mesh = TestHelpers::CreateValidMesh();

	// Get vertex data
	Vertex& vertexData0 = mesh.GetVertexData(0);
	EXPECT_EQ(vertexData0.Position, (Vec3{ 0., 0., 0. }));

	// Modify vertex data
	vertexData0.Position = { 2., 2., 2. };
	EXPECT_EQ(mesh.GetVertexData(0).Position, (Vec3{ 2., 2., 2. }));

	// Get triangle data
	Triangle& faceData = mesh.GetTriangleData(0);
	EXPECT_EQ(faceData.Vertices[0], 0);
	EXPECT_EQ(faceData.Vertices[1], 1);
	EXPECT_EQ(faceData.Vertices[2], 2);

	// Modify triangle data
	faceData.Vertices[0] = 3;
	EXPECT_EQ(mesh.GetTriangleData(0).Vertices[0], 3);
}

TEST(MeshTest, GetConstVertexAndFaceData_ShouldReturnReferences)
{
	const Mesh mesh = TestHelpers::CreateValidMesh();

	// Get vertex data
	const Vertex& vertexData1 = mesh.GetVertexData(1);
	EXPECT_EQ(vertexData1.Position, (Vec3{ 1., 0., 0. }));

	// Get triangle data
	const Triangle& faceData1 = mesh.GetTriangleData(1);
	EXPECT_EQ(faceData1.Vertices[0], 0);
	EXPECT_EQ(faceData1.Vertices[1], 2);
	EXPECT_EQ(faceData1.Vertices[2], 3);
}

TEST(MeshTest, AddExtraDataContainers_ShouldAddContainers)
{
	Mesh mesh = TestHelpers::CreateValidMesh();

	// Initially, the mesh should not have extra data containers
	EXPECT_FALSE(mesh.HasVerticesExtraDataContainer());
	EXPECT_FALSE(mesh.HasTrianglesExtraDataContainer());

	// Add extra data containers
	mesh.AddVerticesExtraDataContainer();
	mesh.AddTrianglesExtraDataContainer();

	// Now, the mesh should have extra data containers
	EXPECT_TRUE(mesh.HasVerticesExtraDataContainer());
	EXPECT_TRUE(mesh.HasTrianglesExtraDataContainer());
}

TEST(MeshTest, Clone_ShouldReturnDeepCopy)
{
	Mesh originalMesh = TestHelpers::CreateValidMesh();

	// Clone the mesh
	std::unique_ptr<Mesh> clonedMesh = originalMesh.Clone();

	// Verify that the cloned mesh has the same number of vertices and faces
	EXPECT_EQ(clonedMesh->GetVertexCount(), originalMesh.GetVertexCount());
	EXPECT_EQ(clonedMesh->GetTriangleCount(), originalMesh.GetTriangleCount());

	// Modify the original mesh and verify that the cloned mesh remains unchanged
	originalMesh.GetVertexData(0).Position = { 2., 2., 2. };
	EXPECT_NE(clonedMesh->GetVertexData(0).Position, originalMesh.GetVertexData(0).Position);

	originalMesh.GetTriangleData(0).Vertices[0] = 3;
	EXPECT_NE(clonedMesh->GetTriangleData(0).Vertices[0], originalMesh.GetTriangleData(0).Vertices[0]);
}

TEST(MeshTest, UpdateMeshConnectivity_ShouldSetNeighborsAndIncidentFaces)
{
	Mesh mesh;

	// Create a simple square mesh with two faces sharing an edge
	mesh.AddVertex({ .Position = { 0., 0., 0. } }); // Vertex 0
	mesh.AddVertex({ .Position = { 1., 0., 0. } }); // Vertex 1
	mesh.AddVertex({ .Position = { 1., 1., 0. } }); // Vertex 2
	mesh.AddVertex({ .Position = { 0., 1., 0. } }); // Vertex 3

	mesh.AddTriangle({ .Vertices = { 0, 1, 2 } }); // Triangle 0
	mesh.AddTriangle({ .Vertices = { 0, 2, 3 } }); // Triangle 1

	// Update connectivity
	mesh.UpdateMeshConnectivity();

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::ExitCode::MeshOK);
}

TEST(MeshTest, AddTriangle_ValidMeshWithED_ShouldAddExtraDataContainer)
{
	Mesh mesh = TestHelpers::CreateValidMeshWithED();

	mesh.AddTriangle({ .Vertices = { 2, 4, 3 } });
	EXPECT_EQ(mesh.GetTriangle(mesh.GetTriangleCount() - 1).GetExtraData<void>(), nullptr);
}

TEST(MeshTest, ComputeTriangleNormals_ShouldComputeEachTriangleNormal)
{
	std::unique_ptr<Mesh> mesh = MeshLoader::LoadOFF("TestFiles/Off/cube.off");
	mesh->ComputeTriangleNormals(true);

	// The cube has 12 triangular faces (2 per cube triangle)
	ASSERT_EQ(mesh->GetTriangleCount(), 12);

	// Expected normals for each cube face (normalized)
	// Each cube face has 2 triangular faces with the same normal
	const std::vector<Vec3> expectedTriangleNormals = {
		Vec3(0, 0, -1), // Triangle 0: Front face (-Z)
		Vec3(0, 0, -1), // Triangle 1: Front face (-Z)
		Vec3(-1, 0, 0), // Triangle 2: Left face (-X)
		Vec3(-1, 0, 0), // Triangle 3: Left face (-X)
		Vec3(1, 0, 0), // Triangle 4: Right face (+X)
		Vec3(1, 0, 0), // Triangle 5: Right face (+X)
		Vec3(0, -1, 0), // Triangle 6: Bottom face (-Y)
		Vec3(0, -1, 0), // Triangle 7: Bottom face (-Y)
		Vec3(0, 0, 1), // Triangle 8: Back face (+Z)
		Vec3(0, 0, 1), // Triangle 9: Back face (+Z)
		Vec3(0, 1, 0), // Triangle 10: Top face (+Y)
		Vec3(0, 1, 0) // Triangle 11: Top face (+Y)
	};

	constexpr float epsilon = 1e-5f;

	// Check that each triangle computed normal matches the expected one.
	for(TriangleIndex iTriangle = 0; iTriangle < mesh->GetTriangleCount(); ++iTriangle)
	{
		// Get the computed normal for the current triangle.
		auto curTriangleExtraData = mesh->GetTriangle(iTriangle).GetExtraData<TriangleNormalExtraData>();
		assert(curTriangleExtraData != nullptr);
		const Vec3& computedNormal = curTriangleExtraData->GetData();

		// Check that the normal matches the expected direction.
		EXPECT_TRUE(EqualNear(computedNormal, expectedTriangleNormals[iTriangle], epsilon));
	}
}

TEST(MeshTest, ComputeSmoothVertexNormals_ShouldComputeEachSmoothVertexNormal)
{
	std::unique_ptr<Mesh> mesh = MeshLoader::LoadOFF("TestFiles/Off/cube.off");
	mesh->ComputeSmoothVertexNormals(true);

	// The cube has 12 triangular faces (2 per cube triangle)
	ASSERT_EQ(mesh->GetTriangleCount(), 12);

	// For a cube, each vertex is shared by 3 faces meeting at 90-degree angles
	// The angle-weighted vertex normal is the sum of triangle normals weighted by their angles
	// Since all angles are 90 degrees (π/2) and faces are orthogonal:
	// Each vertex normal = normalized sum of 3 orthogonal face normals
	// This results in normals pointing along the diagonal from cube center to each vertex

	const float invSqrt3 = 1.0f / std::sqrt(3.0f); // ≈ 0.57735

	const std::vector<Vec3> expectedVertexNormals = {
		Vec3(-invSqrt3, -invSqrt3, -invSqrt3), // Vertex 0: (-1, -1, -1)
		Vec3(-invSqrt3, invSqrt3, -invSqrt3), // Vertex 1: (-1, 1, -1)
		Vec3(invSqrt3, invSqrt3, -invSqrt3), // Vertex 2: (1, 1, -1)
		Vec3(invSqrt3, -invSqrt3, -invSqrt3), // Vertex 3: (1, -1, -1)
		Vec3(-invSqrt3, -invSqrt3, invSqrt3), // Vertex 4: (-1, -1, 1)
		Vec3(-invSqrt3, invSqrt3, invSqrt3), // Vertex 5: (-1, 1, 1)
		Vec3(invSqrt3, invSqrt3, invSqrt3), // Vertex 6: (1, 1, 1)
		Vec3(invSqrt3, -invSqrt3, invSqrt3) // Vertex 7: (1, -1, 1)
	};

	constexpr float epsilon = 1e-5f;

	// Check that each smooth vertex computed normal matches the expected one.
	for(VertexIndex iVertex = 0; iVertex < mesh->GetVertexCount(); ++iVertex)
	{
		// Get the computed normal for the current vertex.
		auto curVertexExtraData = mesh->GetVertex(iVertex).GetExtraData<SmoothVertexNormalExtraData>();
		assert(curVertexExtraData != nullptr);
		const Vec3& computedNormal = curVertexExtraData->GetData();

		// Check that the normal matches the expected direction.
		EXPECT_TRUE(EqualNear(computedNormal, expectedVertexNormals[iVertex], epsilon));
	}
}

TEST(MeshTest, UpdateVerticesBoudaryStatus_ShouldUpdateEachVertexBoundaryStatus)
{
	Mesh mesh = TestHelpers::CreateGridMesh(2, 2);

	mesh.UpdateVerticesBoundaryStatus();

	EXPECT_TRUE(mesh.GetVertex(0).GetExtraData<IsBoundaryVertexExtraData>()->IsBoundary());
	EXPECT_TRUE(mesh.GetVertex(1).GetExtraData<IsBoundaryVertexExtraData>()->IsBoundary());
	EXPECT_TRUE(mesh.GetVertex(2).GetExtraData<IsBoundaryVertexExtraData>()->IsBoundary());
	EXPECT_TRUE(mesh.GetVertex(3).GetExtraData<IsBoundaryVertexExtraData>()->IsBoundary());
	EXPECT_FALSE(mesh.GetVertex(4).GetExtraData<IsBoundaryVertexExtraData>()->IsBoundary());
	EXPECT_TRUE(mesh.GetVertex(5).GetExtraData<IsBoundaryVertexExtraData>()->IsBoundary());
	EXPECT_TRUE(mesh.GetVertex(6).GetExtraData<IsBoundaryVertexExtraData>()->IsBoundary());
	EXPECT_TRUE(mesh.GetVertex(7).GetExtraData<IsBoundaryVertexExtraData>()->IsBoundary());
	EXPECT_TRUE(mesh.GetVertex(8).GetExtraData<IsBoundaryVertexExtraData>()->IsBoundary());
}

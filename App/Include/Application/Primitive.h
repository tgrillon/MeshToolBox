#pragma once

#include "Core/BaseTypes.h"

#include <array>

/// @brief Helper namespace for index manipulations in triangles.
namespace IndexHelpers
{
/// @brief Arrays to get the previous index in a triangle.
constexpr std::array<Core::BaseType::EdgeIndex, 3> Previous{ 2, 0, 1 };
/// @brief Arrays to get the current index in a triangle.
constexpr std::array<Core::BaseType::EdgeIndex, 3> Current{ 0, 1, 2 };
/// @brief Arrays to get the next index in a triangle.
constexpr std::array<Core::BaseType::EdgeIndex, 3> Next{ 1, 2, 0 };
} // namespace IndexHelpers

namespace Data::Primitive
{
struct Triangle;

/// @brief Structure holding the informations relative to a vertex primitive.
struct Vertex
{
	/// @brief Vertex position in 3D.
	Core::BaseType::Vec3 Position{};

	/// @brief Index to one of the incident triangles for internal purposes.
	/// @note If the vertex has no incident triangle, this index is set to -1.
	int IncidentTriangleIdx{ -1 };
};

/// @brief  Structure holding the informations relative to a triangular triangle primitive.
struct Triangle
{
	/// @brief Triangle vertex indices.
	/// @note Vertices must be stored in counter-clockwise order.
	/// @note The triangle wont be valid if one of the indices is -1.
	std::array<int, 3> Vertices{ -1, -1, -1 };

	/// @brief Triangle neighboring triangle indices.
	/// @note Neighbors[i] is the neighbor opposite to Vertices[i].
	/// @note If the triangle has no neighbor on the edge, the index is set to -1.
	std::array<int, 3> Neighbors{ -1, -1, -1 };
};
} // namespace Data::Primitive

namespace Utilitary::Primitive
{
/// @brief Get the index of the edge defined by two vertices in a triangle.
/// @param triangle The triangle containing the edge.
/// @param firstIndex The first vertex index of the edge.
/// @param secondIndex The second vertex index of the edge.
/// @return The index of the edge (0, 1, or 2), or -1 if the edge is not found.
int GetEdgeIndex(
	const Data::Primitive::Triangle& triangle,
	const Core::BaseType::VertexIndex firstIndex,
	const Core::BaseType::VertexIndex secondIndex);

/// @brief Get the local index of a vertex in a triangle.
/// @param triangle The triangle containing the vertex.
/// @param index The vertex to found in the triangle.
/// @return The local index of the vertex (0, 1, or 2), or -1 if it is not found.
int GetVertexLocalIndex(const Data::Primitive::Triangle& triangle, const Core::BaseType::VertexIndex index);
} // namespace Utilitary::Primitive

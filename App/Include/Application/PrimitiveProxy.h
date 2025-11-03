#pragma once

#include "Application/Mesh.h"
#include "Core/BaseTypes.h"

namespace Data::Primitive
{
/// @brief Proxy class for a triangle in a mesh, providing safe access and extra data storage.
class TriangleProxy
{
public:
	/// @brief Construct a TriangleProxy from a mesh and a triangle.
	TriangleProxy(Data::Surface::Mesh& mesh, const Core::BaseType::TriangleIndex index);

	/// @brief Get extra data of type T associated with the triangle, or nullptr if not found.
	template<typename T>
	T* GetExtraData()
	{
		assert(m_Mesh->HasTrianglesExtraDataContainer());
		return m_Mesh->m_TrianglesExtraDataContainer[m_Index].Get<T>();
	}

	/// @brief Get extra data of type T associated with the triangle, or nullptr if not found.
	template<typename T>
	const T* GetExtraData() const
	{
		if(!m_Mesh->HasTrianglesExtraDataContainer())
			return nullptr;
		return m_Mesh->m_TrianglesExtraDataContainer[m_Index].Get<const T>();
	}

	/// @brief Get or create extra data of type T associated with the triangle.
	template<typename T>
	T& GetOrCreateExtraData() const
	{
		assert(m_Mesh->HasTrianglesExtraDataContainer());
		return m_Mesh->m_TrianglesExtraDataContainer[m_Index].GetOrCreate<T>();
	}

	/// @brief Set extra data of type T associated with the triangle.
	template<typename T>
	void SetExtraData(T&& data)
	{
		assert(m_Mesh->HasTrianglesExtraDataContainer());
		return m_Mesh->m_TrianglesExtraDataContainer[m_Index].Set<T>(data);
	}

	/// @brief Erase extra data of type T associated with the triangle.
	template<typename T>
	void EraseExtraData()
	{
		assert(m_Mesh->HasTrianglesExtraDataContainer() && HasExtraData<T>());
		return m_Mesh->m_TrianglesExtraDataContainer[m_Index].Erase<T>();
	}

	/// @brief Check if extra data of type T is associated with the triangle.
	template<typename T>
	bool HasExtraData() const
	{
		return m_Mesh->HasTrianglesExtraDataContainer() && m_Mesh->m_TrianglesExtraDataContainer[m_Index].Has<T>();
	}

	/// @brief Get the index of the triangle in the mesh.
	Core::BaseType::TriangleIndex GetIndex() const;

	/// @brief Get the triangle being proxied.
	Triangle& GetTriangle();
	/// @brief Get the triangle being proxied (const version).
	const Triangle& GetTriangle() const;

	/// @brief Get the vertex at the given index of the triangle.
	Core::BaseType::VertexIndex GetVertex(const Core::BaseType::EdgeIndex index) const;

	/// @brief Get all vertex indices of the triangle.
	std::array<int, 3> GetVertices() const;

	/// @brief Get the neighbor triangle at the given index of the triangle.
	int GetNeighbor(const Core::BaseType::EdgeIndex index) const;

	/// @brief Get all neighbor triangle indices of the triangle.
	std::array<int, 3> GetNeighbors() const;

private:
	/// @brief Pointer to the mesh containing the triangle.
	mutable Data::Surface::Mesh* m_Mesh;
	/// @brief Index to the triangle being proxied.
	Core::BaseType::TriangleIndex m_Index;
};

/// @brief Proxy class for a vertex in a mesh, providing safe access and extra data storage.
class VertexProxy
{
public:
	/// @brief Construct a VertexProxy from a mesh and a vertex.
	VertexProxy(Data::Surface::Mesh& mesh, const Core::BaseType::VertexIndex index);

	/// @brief Get extra data of type T associated with the vertex, or nullptr if not found.
	template<typename T>
	T* GetExtraData()
	{
		if(!m_Mesh->HasVerticesExtraDataContainer())
			return nullptr;
		return m_Mesh->m_VerticesExtraDataContainer[m_Index].Get<T>();
	}

	/// @brief Get extra data of type T associated with the vertex, or nullptr if not found.
	template<typename T>
	const T* GetExtraData() const
	{
		assert(m_Mesh->HasVerticesExtraDataContainer());
		return m_Mesh->m_VerticesExtraDataContainer[m_Index].Get<T>();
	}

	/// @brief Get or create extra data of type T associated with the vertex.
	template<typename T>
	T& GetOrCreateExtraData() const
	{
		assert(m_Mesh->HasVerticesExtraDataContainer());
		return m_Mesh->m_VerticesExtraDataContainer[m_Index].GetOrCreate<T>();
	}

	/// @brief Set extra data of type T associated with the vertex.
	template<typename T>
	void SetExtraData(T&& data)
	{
		assert(m_Mesh->HasVerticesExtraDataContainer());
		return m_Mesh->m_VerticesExtraDataContainer[m_Index].Set<T>(data);
	}

	/// @brief Check if extra data of type T is associated with the vertex.
	template<typename T>
	bool HasExtraData() const
	{
		return m_Mesh->HasVerticesExtraDataContainer() && m_Mesh->m_VerticesExtraDataContainer[m_Index].Has<T>();
	}

	/// @brief Erase extra data of type T associated with the vertex.
	template<typename T>
	void EraseExtraData() const
	{
		assert(m_Mesh->HasVerticesExtraDataContainer() && HasExtraData<T>());
		return m_Mesh->m_VerticesExtraDataContainer[m_Index].Erase<T>();
	}

	/// @brief Get the index of the vertex in the mesh.
	Core::BaseType::VertexIndex GetIndex() const;

	/// @brief Get the vertex being proxied.
	Vertex& GetVertex();
	/// @brief Get the vertex being proxied (const version).
	const Vertex& GetVertex() const;

	/// @brief Get the position of the vertex.
	Core::BaseType::Vec3& GetPosition();

	/// @brief Get the position of the vertex (const version).
	const Core::BaseType::Vec3& GetPosition() const;

	/// @brief Get the incident triangle of the vertex.
	int GetIncidentTriangle() const;

private:
	/// @brief Pointer to the mesh containing the vertex.
	Data::Surface::Mesh* m_Mesh;
	/// @brief Index to the vertex being proxied.
	Core::BaseType::VertexIndex m_Index;
};
} // namespace Data::Primitive

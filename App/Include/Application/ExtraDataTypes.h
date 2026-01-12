#pragma once

#include "Core/BaseTypes.h"

#include <array>
#include <string>
#include <vector>

namespace Data::ExtraData
{
/// @brief Base class for extra data type.
class BaseExtraDataType
{
public:
	virtual ~BaseExtraDataType() = default;

	/// @brief Returns the name of the extra data.
	virtual std::string GetName() = 0;
};

/// @brief Base class for extra data type with only one attribute.
/// @note Any extra data type with only one attribute should derives from this class.
template<typename T>
class SingleDataExtraData : public BaseExtraDataType
{
public:
	SingleDataExtraData() = default;
	~SingleDataExtraData() = default;

	/// @brief Getter on the data stored (const version).
	T const& GetData() const { return m_Data; }

	/// @brief Getter on the data stored.
	T& GetData() { return m_Data; }

	/// @brief Setter on the data stored.
	void SetData(T const& value) { m_Data = value; }

	/// @brief Setter on the data stored.
	void SetData(T&& value) { m_Data = value; }

private:
	T m_Data;
};

/// @brief Extra data type to store a triangle normal.
class TriangleNormalExtraData : public SingleDataExtraData<Core::BaseType::Vec3>
{
public:
	/// @brief Returns the name of the extra data.
	std::string GetName() override { return "TriangleNormalExtraData"; }
};

/// @brief Extra data type to store a smooth vertex normal.
class SmoothVertexNormalExtraData : public SingleDataExtraData<Core::BaseType::Vec3>
{
public:
	/// @brief Returns the name of the extra data.
	std::string GetName() override { return "SmoothVertexNormalExtraData"; }
};

/// @brief Extra data type to store vertex flat normals.
class FlatVertexNormalsExtraData : public SingleDataExtraData<std::vector<Core::BaseType::Vec3>>
{
public:
	/// @brief Returns the name of the extra data.
	std::string GetName() override { return "FlatVertexNormalsExtraData"; }
};

/// @brief Extra data type to store texture coordinates for each vertex of a triangle.
class VerticesTexCoordsExtraData : public SingleDataExtraData<std::array<Core::BaseType::Vec2, 3>>
{
public:
	/// @brief Returns the name of the extra data.
	std::string GetName() override { return "VertexTexCoordsExtraData"; }

	const Core::BaseType::Vec2& GetVertexTexCoords(const Core::BaseType::VertexLocalIndex index) const
	{
		return GetData()[index];
	}

	Core::BaseType::Vec2& GetVertexTexCoords(const Core::BaseType::VertexLocalIndex index) { return GetData()[index]; }

	void SetVertexTexCoords(const Core::BaseType::Vec2& value, const Core::BaseType::VertexLocalIndex index)
	{
		GetData()[index] = value;
	}
};

/// @brief Extra data type to store a boolean that tells if the vertex is ona boundary of the mesh.
class IsBoundaryVertexExtraData : public SingleDataExtraData<bool>
{
public:
	/// @brief Returns the name of the extra data.
	std::string GetName() override { return "VertexFreeAttrib"; }

	/// @brief Returns true if the vertex is on a boundary, false otherwise.
	bool IsBoundary() const { return GetData(); }
};

/// @brief Extra data type to store a triangle material name.
class TriangleMaterialName : public SingleDataExtraData<std::string>

{
public:
	/// @brief Returns the name of the extra data.
	std::string GetName() override { return "TriangleMaterialName"; }
};

/// @brief Store object material data defined within a MTL file.
/// @note For now, LoadOBJ ignore the material informations.
class ObjectMaterialData : public BaseExtraDataType
{
public:
	ObjectMaterialData() = default;
	~ObjectMaterialData() = default;

	/// @brief Returns the name of the extra data.
	std::string GetName() override { return "ObjectMaterialData"; }

public:
	/// @brief Ambiant color.
	Core::BaseType::Vec3 Ka{ 0., 0., 0. };
	/// @brief Diffuse color.
	Core::BaseType::Vec3 Kd{ 0., 0., 0. };
	/// @brief Specular color.
	Core::BaseType::Vec3 Ks{ 0., 0., 0. };
	/// @brief Emissive color.
	Core::BaseType::Vec3 Ke{ 0., 0., 0. };

	/// @brief Specular exponent.
	float Ns{ 0. };
	/// @brief Optical density.
	float Ni{ 0. };
	/// @brief Opacity.
	float D{ 1.0 };

	/// @brief Texture used with ambiant color.
	std::string MapKa{};
	/// @brief Texture used with diffuse color.
	std::string MapKd{};
	/// @brief Texture used with specular color.
	std::string MapKs{};
};

} // namespace Data::ExtraData

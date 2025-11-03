#pragma once

#include "Core/BaseType.h"

#include <cmath>
#include <limits>

namespace Core::Math::Compare
{
/// @brief Helper function to compare two floating point variables.
constexpr bool EqualNear(float lhs, float rhs, float eps = 1e-6)
{
	float diff = std::fabs(lhs - rhs);
	if(diff <= eps)
		return true;

	return diff <= (std::max(std::fabs(lhs), std::fabs(rhs)) * eps);
}

/// @brief Helper function to compare two Vec2 variables .
constexpr bool EqualNear(BaseType::Vec2 lhs, BaseType::Vec2 rhs, float eps = 1e-6)
{
	return EqualNear(lhs.x, rhs.x, eps) && EqualNear(lhs.y, rhs.y, eps);
}

/// @brief Helper function to compare two Vec3 variables.
constexpr bool EqualNear(BaseType::Vec3 lhs, BaseType::Vec3 rhs, float eps = 1e-6)
{
	return EqualNear(lhs.x, rhs.x, eps) && EqualNear(lhs.y, rhs.y, eps) && EqualNear(lhs.z, rhs.z, eps);
}
} // namespace Core::Math::Compare

namespace Core::Math::Geometry
{
/// @brief Returns a vector in the same direction as v but with length of 1.
/// @warning According to issue 10 GLSL 1.10 specification, if length(x) == 0 then result is undefined and generate an error.
inline BaseType::Vec3 Normalize(const BaseType::Vec3& v)
{
	return glm::normalize(v);
}

/// @brief Returns a quaternion in the same direction as q but with length of 1.
/// @warning According to issue 10 GLSL 1.10 specification, if length(x) == 0 then result is undefined and generate an error.
inline BaseType::Quat Normalize(const BaseType::Quat& q)
{
	return glm::normalize(q);
}

/// @brief Returns the length of v, i.e., sqrt(v * v).
inline BaseType::ScalarValue Length(const BaseType::Vec3& v)
{
	return glm::length(v);
}

/// @brief Returns the cross product of u and v.
inline BaseType::Vec3 Cross(const BaseType::Vec3& u, const BaseType::Vec3& v)
{
	return glm::cross(u, v);
}

/// @brief Returns the cross product of p and q.
inline BaseType::Quat Cross(const BaseType::Quat& p, const BaseType::Quat& q)
{
	return glm::cross(p, q);
}

/// @brief Returns the dot product of u and v, i.e., result = u * v.
inline BaseType::ScalarValue Dot(const BaseType::Vec3& u, const BaseType::Vec3& v)
{
	return glm::dot(u, v);
}

/// @brief Returns the absolute angle between two vectors.
/// @note Parameters need to be normalized.
inline BaseType::ScalarValue Angle(const BaseType::Vec3& u, const BaseType::Vec3& v)
{
	return glm::angle(u, v);
}

/// @brief Build a quaternion from an angle and a normalized axis.
inline BaseType::Quat AngleAxis(const BaseType::ScalarValue angle, const BaseType::Vec3& axis)
{
	return glm::angleAxis(angle, axis);
}

/// @brief Builds a rotation 3 * 3 matrix created from an angle.
inline BaseType::Vec3 Rotate(const BaseType::Quat& q, const BaseType::Vec3& v)
{
	return glm::rotate(q, v);
}

/// @brief Creates a matrix for a symetric perspective-view frustum based on the default handedness and default near and far clip planes definition.
inline BaseType::Mat4 Perspective(
	const BaseType::ScalarValue fov,
	const BaseType::ScalarValue aspect,
	const BaseType::ScalarValue zNear,
	const BaseType::ScalarValue zFar)
{
	return glm::perspective(fov, aspect, zNear, zFar);
}

/// @brief Build a look at view matrix based on the default handedness.
inline BaseType::Mat4 LookAt(const BaseType::Vec3& eye, const BaseType::Vec3& center, const BaseType::Vec3& up)
{
	return glm::lookAt(eye, center, up);
}

/// @brief Build a look at view matrix based on the default handedness.
inline BaseType::Mat4 Ortho(
	const BaseType::ScalarValue& left,
	const BaseType::ScalarValue right,
	const BaseType::ScalarValue bottom,
	const BaseType::ScalarValue top,
	const BaseType::ScalarValue zNear,
	const BaseType::ScalarValue zFar)
{
	return glm::ortho(left, right, bottom, top, zNear, zFar);
}
} // namespace Core::Math::Geometry

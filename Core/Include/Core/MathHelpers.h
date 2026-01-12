#pragma once

#include "Core/BaseTypes.h"

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
/// @note fov must be expressed in radians.
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

/// @brief Get the center of the segment AB.
inline BaseType::Vec3 Center(const BaseType::Vec3& a, const BaseType::Vec3& b)
{
	const BaseType::ScalarValue x = (a.x + b.x) * 0.5;
	const BaseType::ScalarValue y = (a.y + b.y) * 0.5;
	const BaseType::ScalarValue z = (a.z + b.z) * 0.5;
	return BaseType::Vec3{ x, y, z };
}

/// @brief Get the distance between two points A and B.
inline BaseType::ScalarValue Distance(const BaseType::Vec3& a, const BaseType::Vec3& b)
{
	return Length(b - a);
}

inline BaseType::Mat4 GetTranslationMatrix(const BaseType::Vec3& t)
{
	return glm::translate(t);
}

inline BaseType::Mat4 GetTranslationMatrix(
	const BaseType::ScalarValue x, const BaseType::ScalarValue y, const BaseType::ScalarValue z)
{
	return GetTranslationMatrix(BaseType::Vec3{ x, y, z });
}

inline BaseType::ScalarValue Radians(const BaseType::ScalarValue value)
{
	return glm::radians(value);
}

inline BaseType::Mat4 IdentityMat4()
{
	return glm::mat4(1);
}

inline BaseType::Mat4 GetRotationXMatrix(const BaseType::ScalarValue angle)
{
	BaseType::Vec3 axis{ 1.0, 0.0, 0.0 };
	return glm::rotate(Radians(angle), axis);
}

inline BaseType::Mat4 GetRotationYMatrix(const BaseType::ScalarValue angle)
{
	BaseType::Vec3 axis{ 0.0, 1.0, 0.0 };
	return glm::rotate(Radians(angle), axis);
}

inline BaseType::Mat4 GetRotationZMatrix(const BaseType::ScalarValue angle)
{
	BaseType::Vec3 axis{ 0.0, 0.0, 1.0 };
	return glm::rotate(Radians(angle), axis);
}

inline BaseType::Mat4 Viewport(const float width, const float height)
{
	float w = width / 2.f;
	float h = height / 2.f;

	return BaseType::Mat4(
		BaseType::Vec4(w, 0, 0, 0), // col 0
		BaseType::Vec4(0, h, 0, 0), // col 1
		BaseType::Vec4(0, 0, 0.5f, 0), // col 2
		BaseType::Vec4(w, h, 0.5f, 1) // col 3
	);
}

inline BaseType::Mat4 Inverse(const BaseType::Mat4& m)
{
	return glm::inverse(m);
}

inline BaseType::Mat4 Translate(const BaseType::Mat4& m, const BaseType::Vec3& v)
{
	return glm::translate(m, v);
}

inline BaseType::Mat4 ToMat4(const BaseType::Quat& q)
{
	return glm::toMat4(q);
}
} // namespace Core::Math::Geometry

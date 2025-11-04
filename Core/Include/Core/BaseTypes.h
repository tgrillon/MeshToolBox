#pragma once

#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <bitset>
#include <cstdint>

namespace Core::BaseType
{
using iVec2 = glm::ivec2;

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;

using Mat4 = glm::mat4;

using Quat = glm::quat;
} // namespace Core::BaseType

namespace std
{
/// @brief Hash function for Vec2 to be used in unordered containers.
template<>
struct hash<Core::BaseType::Vec2>
{
	size_t operator()(const Core::BaseType::Vec2& elt) const
	{
		auto h1 = std::hash<float>{}(elt.x);
		auto h2 = std::hash<float>{}(elt.y);
		return h1 ^ h2;
	}
};

/// @brief Hash function for Vec3 to be used in unordered containers.
template<>
struct hash<Core::BaseType::Vec3>
{
	size_t operator()(const Core::BaseType::Vec3& elt) const
	{
		auto h1 = std::hash<float>{}(elt.x);
		auto h2 = std::hash<float>{}(elt.y);
		auto h3 = std::hash<float>{}(elt.z);
		return h1 ^ h2 ^ h3;
	}
};
} // namespace std

/*
 camera.h
 OpenGL Camera Code
 Capable of 2 modes, orthogonal, and free
 Quaternion camera code adapted from: http://hamelot.co.uk/visualization/opengl-camera/
 Written by Hammad Mazhar
 */
#pragma once

#include "Core/BaseTypes.h"
#include "Core/Event/Event.h"
#include "Core/Event/KeyEvents.h"
#include "Core/Event/MouseEvents.h"
#include "Core/MathHelpers.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>

namespace Core
{
class Camera
{
public:
	Camera() = default;
	Camera(float fov, float aspectRatio, float nearClip, float farClip);

	void OnUpdate(const float ts);
	void OnEvent(Event& e);

	inline float GetDistance() const { return m_Distance; }

	inline void SetDistance(float distance) { m_Distance = distance; }

	inline void SetViewportSize(float width, float height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		UpdateProjection();
	}

	const Core::BaseType::Mat4& GetViewMatrix() const { return m_View; }

	Core::BaseType::Mat4 GetViewProjection() const { return m_Projection * m_View; }

	Core::BaseType::Vec3 GetUpDirection() const;
	Core::BaseType::Vec3 GetRightDirection() const;
	Core::BaseType::Vec3 GetForwardDirection() const;

	const Core::BaseType::Vec3& GetPosition() const { return m_Position; }

	glm::quat GetOrientation() const;

	float GetPitch() const { return m_Pitch; }

	float GetYaw() const { return m_Yaw; }

private:
	void UpdateProjection();
	void UpdateView();

	bool OnMouseScroll(MouseScrolledEvent& e);

	void MousePan(const Core::BaseType::Vec2& delta);
	void MouseRotate(const Core::BaseType::Vec2& delta);
	void MouseZoom(float delta);

	Core::BaseType::Vec3 CalculatePosition() const;

	std::pair<float, float> PanSpeed() const;
	float RotationSpeed() const;
	float ZoomSpeed() const;

private:
	float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

	Core::BaseType::Mat4 m_View;
	Core::BaseType::Mat4 m_Projection;
	Core::BaseType::Vec3 m_Position = { 0.0f, 0.0f, 0.0f };
	Core::BaseType::Vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

	Core::BaseType::Vec2 m_InitialMousePosition = { 0.0f, 0.0f };

	Core::BaseType::Vec2 m_Translation = { 0.0f, 0.0f };

	float m_Distance = 10.0f;
	float m_Pitch = 0.0f, m_Yaw = 0.0f;

	float m_ViewportWidth = 1280, m_ViewportHeight = 720;
};

} // namespace Core

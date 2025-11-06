
#include "Core/Camera.h"

#include "Core/Event/Input.h"
#include "Core/MathHelpers.h"

#include <algorithm>
#include <cstdio>

using namespace Core::BaseType;
using namespace Core::Math::Geometry;

namespace Core
{

Camera::Camera(float fov, float aspectRatio, float nearClip, float farClip)
	: m_FOV(fov)
	, m_AspectRatio(aspectRatio)
	, m_NearClip(nearClip)
	, m_FarClip(farClip)
	, m_Projection(Perspective(Radians(fov), aspectRatio, nearClip, farClip))
{
	UpdateView();
}

void Camera::UpdateProjection()
{
	m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
	m_Projection = Perspective(Radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	m_Projection[1][1] *= -1; // Flip Y for OpenGL
}

void Camera::UpdateView()
{
	m_Position = CalculatePosition();

	Quat orientation = GetOrientation();
	m_View = Translate(IdentityMat4(), m_Position) * ToMat4(orientation);
	m_View = Inverse(m_View);
}

std::pair<float, float> Camera::PanSpeed() const
{
	float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
	float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

	float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
	float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

	return { xFactor, yFactor };
}

float Camera::RotationSpeed() const
{
	return 0.8f;
}

float Camera::ZoomSpeed() const
{
	float distance = m_Distance * 0.2f;
	distance = std::max(distance, 0.0f);
	float speed = distance * distance;
	speed = std::min(speed, 100.0f); // max speed = 100
	return speed;
}

void Camera::OnUpdate(const float ts)
{
	const Vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
	Vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
	m_InitialMousePosition = mouse;

	if(Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
		MousePan(delta);
	else if(Input::IsMouseButtonPressed(Mouse::ButtonLeft))
		MouseRotate(delta);
	else if(Input::IsMouseButtonPressed(Mouse::ButtonRight))
	{
		if(std::abs(delta.x) > std::abs(delta.y * m_AspectRatio))
			MouseZoom(delta.x);
		else
			MouseZoom(delta.y);
	}

	UpdateView();
}

void Camera::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(Camera::OnMouseScroll));
}

bool Camera::OnMouseScroll(MouseScrolledEvent& e)
{
	float delta = e.GetYOffset() * 0.1f;
	MouseZoom(delta);
	UpdateView();
	return false;
}

void Camera::MousePan(const Vec2& delta)
{
	auto [xSpeed, ySpeed] = PanSpeed();
	m_Translation.x += -delta.x * xSpeed * m_Distance;
	m_Translation.y += delta.y * ySpeed * m_Distance;
}

void Camera::MouseRotate(const Vec2& delta)
{
	float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
	m_Yaw += yawSign * delta.x * RotationSpeed();
	m_Pitch += delta.y * RotationSpeed();
}

void Camera::MouseZoom(float delta)
{
	m_Distance -= delta * ZoomSpeed();
	if(m_Distance < 1.0f)
	{
		m_FocalPoint += GetForwardDirection();
		m_Distance = 1.0f;
	}
}

Vec3 Camera::GetUpDirection() const
{
	return Rotate(GetOrientation(), Vec3(0.0f, 1.0f, 0.0f));
}

Vec3 Camera::GetRightDirection() const
{
	return Rotate(GetOrientation(), Vec3(1.0f, 0.0f, 0.0f));
}

Vec3 Camera::GetForwardDirection() const
{
	return Rotate(GetOrientation(), Vec3(0.0f, 0.0f, -1.0f));
}

Vec3 Camera::CalculatePosition() const
{
	Vec3 position = m_FocalPoint - GetForwardDirection() * m_Distance;
	position += GetRightDirection() * m_Translation.x;
	position += GetUpDirection() * m_Translation.y;
	return position;
}

Quat Camera::GetOrientation() const
{
	return Quat(Vec3(-m_Pitch, -m_Yaw, 0.0f));
}
} // namespace Core

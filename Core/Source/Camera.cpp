#include "Core/Camera.h"

namespace Core
{

using namespace BaseType;
using namespace Math::Geometry;

Camera::Camera()
	: m_CameraMode(CameraMode::FREE)
	, m_CameraUp(Vec3(0, 1, 0))
	, m_FOV(45)
	, m_CameraPositionDelta(Vec3(0, 0, 0))
	, m_CameraScale(.5f)
	, m_MaxPitchRate(5)
	, m_MaxHeadingRate(5)
	, m_MoveCamera(false)
{}

Camera::~Camera()
{}

void Camera::Reset()
{
	m_CameraUp = Vec3(0, 1, 0);
}

void Camera::Update()
{
	m_CameraDirection = Normalize(m_CameraLookAt - m_CameraPosition);
	//need to set the matrix state. this is only important because lighting doesn't work if this isn't done
	glViewport(m_ViewportX, m_ViewportY, m_WindowWidth, m_WindowHeight);

	if(m_CameraMode == CameraMode::ORTHO)
	{
		//our m_Projection matrix will be an orthogonal one in this case
		//if the values are not floating point, this command does not work properly
		//need to multiply by m_AspectRatio!!! (otherise will not scale properly)
		m_Projection = Ortho(-1.5f * float(m_AspectRatio), 1.5f * float(m_AspectRatio), -1.5f, 1.5f, -10.0f, 10.f);
	}
	else if(m_CameraMode == CameraMode::FREE)
	{
		m_Projection = Perspective(m_FOV, m_AspectRatio, m_NearClip, m_FarClip);
		//detmine axis for pitch rotation
		Vec3 axis = Cross(m_CameraDirection, m_CameraUp);
		//compute quaternion for pitch based on the camera pitch angle
		Quat pitch_quat = AngleAxis(m_CameraPitch, axis);
		//determine heading quaternion from the camera up vector and the heading angle
		Quat heading_quat = AngleAxis(m_CameraHeading, m_CameraUp);
		//add the two quaternions
		Quat temp = Cross(pitch_quat, heading_quat);
		temp = Normalize(temp);
		//update the direction from the quaternion
		m_CameraDirection = Rotate(temp, m_CameraDirection);
		//add the camera delta
		m_CameraPosition += m_CameraPositionDelta;
		//set the look at to be infront of the camera
		m_CameraLookAt = m_CameraPosition + m_CameraDirection * 1.0f;
		//damping for smooth camera
		m_CameraHeading *= .5;
		m_CameraPitch *= .5;
		m_CameraPositionDelta = m_CameraPositionDelta * .8f;
	}
	//compute the m_MVP
	m_View = LookAt(m_CameraPosition, m_CameraLookAt, m_CameraUp);
	m_Model = Mat4(1.0f);
	m_MVP = m_Projection * m_View * m_Model;
}

//Setting Functions
void Camera::SetMode(CameraMode cam_mode)
{
	m_CameraMode = cam_mode;
	m_CameraUp = Vec3(0, 1, 0);
}

void Camera::SetPosition(Vec3 pos)
{
	m_CameraPosition = pos;
}

void Camera::SetLookAt(Vec3 pos)
{
	m_CameraLookAt = pos;
}

void Camera::SetFOV(double fov)
{
	m_FOV = fov;
}

void Camera::SetViewport(int loc_x, int loc_y, int width, int height)
{
	m_ViewportX = loc_x;
	m_ViewportY = loc_y;
	m_WindowWidth = width;
	m_WindowHeight = height;
	//need to use doubles division here, it will not work otherwise and it is possible to get a zero m_AspectRatio ratio with integer rounding
	m_AspectRatio = double(width) / double(height);
	;
}

void Camera::SetClipping(double near_clip_distance, double far_clip_distance)
{
	m_NearClip = near_clip_distance;
	m_FarClip = far_clip_distance;
}

void Camera::Move(CameraDirection dir)
{
	if(m_CameraMode == CameraMode::FREE)
	{
		switch(dir)
		{
			case CameraDirection::UP:
				m_CameraPositionDelta += m_CameraUp * m_CameraScale;
				break;
			case CameraDirection::DOWN:
				m_CameraPositionDelta -= m_CameraUp * m_CameraScale;
				break;
			case CameraDirection::LEFT:
				m_CameraPositionDelta -= Cross(m_CameraDirection, m_CameraUp) * m_CameraScale;
				break;
			case CameraDirection::RIGHT:
				m_CameraPositionDelta += Cross(m_CameraDirection, m_CameraUp) * m_CameraScale;
				break;
			case CameraDirection::FORWARD:
				m_CameraPositionDelta += m_CameraDirection * m_CameraScale;
				break;
			case CameraDirection::BACK:
				m_CameraPositionDelta -= m_CameraDirection * m_CameraScale;
				break;
		}
	}
}

void Camera::ChangePitch(float degrees)
{
	//Check bounds with the max pitch rate so that we aren't moving too fast
	if(degrees < -m_MaxPitchRate)
	{
		degrees = -m_MaxPitchRate;
	}
	else if(degrees > m_MaxPitchRate)
	{
		degrees = m_MaxPitchRate;
	}
	m_CameraPitch += degrees;

	//Check bounds for the camera pitch
	if(m_CameraPitch > 360.0f)
	{
		m_CameraPitch -= 360.0f;
	}
	else if(m_CameraPitch < -360.0f)
	{
		m_CameraPitch += 360.0f;
	}
}

void Camera::ChangeHeading(float degrees)
{
	//Check bounds with the max heading rate so that we aren't moving too fast
	if(degrees < -m_MaxHeadingRate)
	{
		degrees = -m_MaxHeadingRate;
	}
	else if(degrees > m_MaxHeadingRate)
	{
		degrees = m_MaxHeadingRate;
	}
	//This controls how the heading is changed if the camera is pointed straight up or down
	//The heading delta direction changes
	if(m_CameraPitch > 90 && m_CameraPitch < 270 || (m_CameraPitch < -90 && m_CameraPitch > -270))
	{
		m_CameraHeading -= degrees;
	}
	else
	{
		m_CameraHeading += degrees;
	}
	//Check bounds for the camera heading
	if(m_CameraHeading > 360.0f)
	{
		m_CameraHeading -= 360.0f;
	}
	else if(m_CameraHeading < -360.0f)
	{
		m_CameraHeading += 360.0f;
	}
}

void Camera::Move2D(int x, int y)
{
	//compute the mouse delta from the previous mouse position
	Vec3 mouse_delta = m_MousePosition - Vec3(x, y, 0);
	//if the camera is moving, meaning that the mouse was clicked and dragged, change the pitch and heading
	if(m_MoveCamera)
	{
		ChangeHeading(.08f * mouse_delta.x);
		ChangePitch(.08f * mouse_delta.y);
	}
	m_MousePosition = Vec3(x, y, 0);
}

void Camera::SetPos(int button, int state, int x, int y)
{
	if(button == 3 && state == GLFW_PRESS)
	{
		m_CameraPositionDelta += m_CameraUp * .05f;
	}
	else if(button == 4 && state == GLFW_PRESS)
	{
		m_CameraPositionDelta -= m_CameraUp * .05f;
	}
	else if(button == GLFW_MOUSE_BUTTON_LEFT && state == GLFW_PRESS)
	{
		m_MoveCamera = true;
	}
	else if(button == GLFW_MOUSE_BUTTON_LEFT && state == GLFW_RELEASE)
	{
		m_MoveCamera = false;
	}
	m_MousePosition = Vec3(x, y, 0);
}

CameraMode Camera::GetMode()
{
	return m_CameraMode;
}

void Camera::GetViewport(int& loc_x, int& loc_y, int& width, int& height)
{
	loc_x = m_ViewportX;
	loc_y = m_ViewportY;
	width = m_WindowWidth;
	height = m_WindowHeight;
}

void Camera::GetMatricies(Mat4& P, Mat4& V, Mat4& M)
{
	P = m_Projection;
	V = m_View;
	M = m_Model;
}
} // namespace Core

/*
 camera.h
 OpenGL Camera Code
 Capable of 2 modes, orthogonal, and free
 Quaternion camera code adapted from: http://hamelot.co.uk/visualization/opengl-camera/
 Written by Hammad Mazhar
 */
#pragma once

#include "Core/BaseType.h"
#include "Core/MathHelpers.h"

#include <GLFW/glfw3.h>

namespace Core
{

enum struct CameraMode : uint8_t
{
	ORTHO = 0,
	FREE
};

enum struct CameraDirection : uint8_t
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT,
	FORWARD,
	BACK
};

/// @brief A quaternion based camera.
class Camera
{
public:
	Camera();
	~Camera();

	void Reset();
	/// @brief This function updates the camera depending on the current camera mode, the m_Projection and viewport matricies are computed, then the position and location of the camera is updated.
	void Update();

	/// @brief Given a specific moving direction, the camera will be moved in the appropriate direction
	/// @brief - for a spherical camera this will be around the look_at point
	/// @brief - for a free camera a delta will be computed for the direction of movement.
	void Move(CameraDirection dir);

	/// @brief Change the pitch (up, down) for the free camera
	void ChangePitch(float degrees);

	/// @brief Change heading (left, right) for the free camera
	void ChangeHeading(float degrees);

	/// @brief Change the heading and pitch of the camera based on the 2d movement of the mouse
	void Move2D(int x, int y);

	/// @brief Changes the camera mode, only three valid modes, Ortho, Free, and Spherical
	void SetMode(CameraMode cam_mode);
	/// @brief Set the position of the camera
	void SetPosition(BaseType::Vec3 pos);
	/// @brief Set's the look at point for the camera
	void SetLookAt(BaseType::Vec3 pos);
	/// @brief Changes the Field of View (FOV) for the camera
	void SetFOV(double fov);
	/// @brief Change the viewport location and size
	void SetViewport(int loc_x, int loc_y, int width, int height);
	/// @brief Change the clipping distance for the camera
	void SetClipping(double near_clip_distance, double far_clip_distance);

	void SetDistance(double cam_dist);
	void SetPos(int button, int state, int x, int y);

	/// @brief Get active camera mode.
	CameraMode GetMode();
	/// @brief Get viewport dimensions.
	void GetViewport(int& loc_x, int& loc_y, int& width, int& height);
	/// @brief Get Projection (P), View (V) and Model (M) matrices.
	void GetMatricies(BaseType::Mat4& P, BaseType::Mat4& V, BaseType::Mat4& M);

private:
	CameraMode m_CameraMode;

	int m_ViewportX;
	int m_ViewportY;

	int m_WindowWidth;
	int m_WindowHeight;

	double m_AspectRatio;
	double m_FOV;
	double m_NearClip;
	double m_FarClip;

	float m_CameraScale;
	float m_CameraHeading;
	float m_CameraPitch;

	float m_MaxPitchRate;
	float m_MaxHeadingRate;
	bool m_MoveCamera;

	BaseType::Vec3 m_CameraPosition;
	BaseType::Vec3 m_CameraPositionDelta;
	BaseType::Vec3 m_CameraLookAt;
	BaseType::Vec3 m_CameraDirection;

	BaseType::Vec3 m_CameraUp;
	BaseType::Vec3 m_MousePosition;

	BaseType::Mat4 m_Projection;
	BaseType::Mat4 m_View;
	BaseType::Mat4 m_Model;
	BaseType::Mat4 m_MVP;
};
} // namespace Core

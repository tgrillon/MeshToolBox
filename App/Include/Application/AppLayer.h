#pragma once

#include "Core/Camera.h"
#include "Core/Layer.h"
#include "Core/Renderer/Renderer.h"

#include <stdint.h>

namespace Application
{
/// @brief Application layer class.
class AppLayer : public Core::Layer
{
public:
	/// @brief Default constructor
	AppLayer();
	/// @brief Default destructor
	virtual ~AppLayer();

	void OnUpdate(float ts) override;
	void OnRender() override;
	void OnEvent(Core::Event& event) override;

private:
	uint32_t m_Shader = 0;

	enum VaoType
	{
		Object = 0,
		CubeMap,
		NbVao
	};

	uint32_t m_Vao[VaoType::NbVao];

	enum VboType
	{
		Position = 0,
		TexCoords,
		Normal,
		Color,
		NbVbo
	};

	uint32_t m_Vbo[VboType::NbVbo];

	Core::Camera m_Camera;

	Renderer::Texture m_Texture;
};

enum struct ActionType
{
	/// SCENE
	DisplayNormals,
	DisplayTriangles,
	DisplayVertices,
	DisplayFaces,
	DisplayEdges,
	DisplayFaceNormals,
	DisplayEdgeAsCylinder,
	DisplayVertexAsSphere,
	DisplayWorldAxis,
	DisplayXYGrid,

	ShowEntireScene,

	InverseNormals,
	ToggleShadingMode,

	IncreaseVertexSize,
	DecreaseVertexSize,
	IncreaseEdgeSize,
	DecreaseEdgeSize,

	/// CAMERA
	RotateCamera,
	TranslateCamera,

	MoveCameraUp,
	MoveCameraLeft,
	MoveCameraRight,
	MoveCameraDown,
	MoveCameraForward,
	MoveCameraBackward,

	ToggleCameraMode,
	ToggleCameraType,
	ToggleCameraConstraintAxis,

	ResetCameraAndClippingPlane,

	IncreaseCameraTranslationSpeed,
	DecreaseCameraTranslationSpeed,
	IncreaseCameraRotationSpeed,
	DecreaseCameraRotationSpeed,

	IncreaseCameraTranslationSmoothness,
	DecreaseCameraTranslationSmoothness,
	IncreaseCameraRotationSmoothness,
	DecreaseCameraRotationSmoothness,

	SetPivotPoint,

	/// CLIPPING PLANE
	RotateClippingPlane,
	TranslateClippingPlane,
	TranslateClippingPlaneAlongCameraDirection,
	TranslateClippingPlaneAlongNormalDirection,

	ToggleClippingPlaneMode,
	ToggleClippingPlaneDisplay,
	ToggleClippingPlaneConstraintAxis,

	IncreaseClippingPlaneTranslationSpeed,
	DecreaseClippingPlaneTranslationSpeed,
	IncreaseClippingPlaneRotationSpeed,
	DecreaseClippingPlaneRotationSpeed,

	ResetClippingPlane,
};
} // namespace Application

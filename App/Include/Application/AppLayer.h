#pragma once

#include "Core/Camera.h"
#include "Core/Event/ApplicationEvent.h"
#include "Core/Layer.h"
#include "Core/Renderer/Renderer.h"

#include <stdint.h>

namespace App
{
/// @brief Application layer class.
class AppLayer : public Core::Layer
{
public:
	/// @brief Default constructor
	AppLayer();
	/// @brief Default destructor
	virtual ~AppLayer();

	void OnUpdate(const float ts) override;
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
} // namespace App

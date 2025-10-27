#pragma once

#include "Core/Camera.h"
#include "Core/Layer.h"

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

	/// @brief Override from Layer
	virtual void OnUpdate(float ts) override;
	/// @brief Override from Layer
	virtual void OnRender() override;

private:
	uint32_t m_Shader = 0;
	uint32_t m_VertexArray = 0;
	uint32_t m_VertexBuffer = 0;

	Core::Camera m_Camera;
};
} // namespace Application

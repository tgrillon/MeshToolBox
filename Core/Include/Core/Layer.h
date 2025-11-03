#pragma once

#include "Core/Event/Event.h"

namespace Core
{

class Layer
{
public:
	virtual ~Layer() = default;

	/// @brief Called to update things each frame.
	virtual void OnUpdate(float) {}

	/// @brief Called to render the layer each frame.
	virtual void OnRender() {}

	/// @brief Called to handle an event each frame.
	virtual void OnEvent(Event&) {}
};

} // namespace Core

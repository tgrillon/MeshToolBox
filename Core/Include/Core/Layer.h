#pragma once

#include "Core/Event/Event.h"

namespace Core
{

class Layer
{
public:
	virtual ~Layer() = default;

	virtual void OnEvent(Event&) {}

	virtual void OnUpdate(float) {}

	virtual void OnRender() {}
};

} // namespace Core

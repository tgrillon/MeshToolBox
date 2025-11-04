#pragma once

#include "Core/BaseTypes.h"
#include "Core/Event/KeyCodes.h"
#include "Core/Event/MouseCodes.h"

namespace Core
{
/// @brief Input handling class.
class Input
{
public:
	/// @brief Checks if the specified key is currently pressed.
	static bool IsKeyPressed(KeyCode key);
	/// @brief Checks if the specified key is currently released.
	static bool IsKeyReleased(KeyCode key);

	/// @brief Checks if the specified mouse button is currently pressed.
	static bool IsMouseButtonPressed(MouseCode button);
	static BaseType::Vec2 GetMousePosition();
	static float GetMouseX();
	static float GetMouseY();
};
} // namespace Core

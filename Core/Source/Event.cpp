#include "Core/Event.h"

#include <GLFW/glfw3.h>

namespace Core
{
using namespace Input;

void EventManager::HandleEvents(const double dt)
{}

void EventManager::RegisterKeyboardAction(
	std::initializer_list<RegularKey> keys,
	std::initializer_list<ModifierKey> modifiers,
	const InputMode mode,
	const ActionType action)
{
	RegisterActionEvents(
		InputBinding{ .Keys = keys, .Modifiers = modifiers, .Mode = mode, .Device = InputDevice::Keyboard }, action);
}

void EventManager::RegisterMouseAction(
	MouseButton button, std::initializer_list<ModifierKey> modifiers, const InputMode mode, const ActionType action)
{
	RegisterActionEvents(
		InputBinding{ .Button = button, .Modifiers = modifiers, .Mode = mode, .Device = InputDevice::Mouse }, action);
}

void EventManager::SetDescriptor(const Descriptor& descriptor)
{
	m_ActionTypeDescriptor = descriptor;
}

void EventManager::SetDescription(const ActionType action, const std::string& section, const std::string& description)
{
	m_ActionTypeDescriptor[section].emplace_back(action, description);
}

bool EventManager::IsKeyPressed(GLFWwindow* window, RegularKey key) const
{
	if(IsAzertyLayout())
		key = MapToQWERTY(key);

	KeyState state = static_cast<KeyState>(glfwGetKey(window, static_cast<int>(key)));

	return state == KeyState::Pressed || state == KeyState::Repeated;
}

bool EventManager::HasEventRunning() const
{
	return m_RegisteredEvents.size() > 0;
}

bool EventManager::IsAzertyLayout() const
{
	return m_KeybordLayout == KeyboardLayout::Qwerty;
}

void EventManager::ToggleKeyboardLayout()
{
	m_KeybordLayout = (m_KeybordLayout == KeyboardLayout::Azerty) ? KeyboardLayout::Qwerty : KeyboardLayout::Azerty;
}

void EventManager::RegisterActionEvents(const InputBinding& binding, ActionType action)
{
	m_RegisteredEvents.insert(Event{ .Action = action, .Binding = binding });

	m_ActionToBindMapping[action].emplace_back(binding);
}

void EventManager::OnKeyEvent(int key, int scancode, int action, int mods)
{
	Input::InputSource input{ .Type = Input::InputSource::InputType::RegularKeyType };
	if(IsAzertyLayout())
		input.Key = MapToQWERTY(static_cast<RegularKey>(key));
	else
		input.Key = static_cast<RegularKey>(key);

	if(action == static_cast<int>(KeyState::Pressed) || action == static_cast<int>(KeyState::Repeated))
	{
		m_InputPressed[input] = true;
		m_InputHeld[input] = true;
	}

	if(action == static_cast<int>(KeyState::Released))
	{
		m_InputHeld.erase(input);
	}
}

void EventManager::OnMouseButtonEvent(int button, int action, int mods)
{
	const KeyState& curAction = static_cast<KeyState>(action);
	Input::InputSource input{ .Type = Input::InputSource::InputType::MouseButtonType };
	if(curAction == KeyState::Pressed)
	{
		auto time = std::chrono::high_resolution_clock::now();

		const std::chrono::duration<double> dt = time - m_LastTimeClick;

		MouseButton curButton = static_cast<MouseButton>(button);
		if(m_LastButtonClicked == curButton && dt < 250ms)
		{
			HandleDoubleClickEvent(static_cast<MouseButton>(curButton));
		}

		m_LastTimeClick = time;
		m_LastButtonClicked = curButton;

		m_InputPressed[input] = true;
		m_InputHeld[input] = true;
	}

	if(curAction == KeyState::Released)
	{
		m_InputHeld.erase(input);
	}
}

void EventManager::OnCursorEvent(double xPos, double yPos, int windowWidth, int windowHeight)
{
	auto& [mouseX, mouseY] = m_MousePosition;
	m_MouseDelta = { xPos - mouseX, yPos - mouseY };
	m_MousePosition = { xPos, yPos };

	m_RollMouseDelta = m_MouseDelta;
	if(yPos < windowHeight / 2)
	{
		m_RollMouseDelta.first *= -1;
	}

	if(xPos < windowWidth / 2)
	{
		m_RollMouseDelta.second *= -1;
	}
}

void EventManager::OnScrollEvent(double xOffset, double yOffset)
{
	m_YOffset = yOffset;
}
} // namespace Core

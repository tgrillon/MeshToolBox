#pragma once

#include "Core/BaseType.h"
#include "Core/KeyCodes.h"

#include <chrono>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std::chrono_literals;

typedef struct GLFWwindow GLFWwindow;

namespace Core
{
enum struct ActionType;

/// @brief Input device for actions.
enum struct InputDevice : uint8_t
{
	Mouse,
	Keyboard
};

/// @brief Input mode for actions.
enum struct InputMode : uint8_t
{
	Hold,
	Release
};

/// @brief Represents an input binding with keys, mode, and device.
struct InputBinding
{
	/// @brief List of keys that make up the binding.
	std::vector<Input::RegularKey> Keys{};

	/// @brief Button that make up the binding.
	Input::MouseButton Button{ Input::MouseButton::Unknown };

	/// @brief List of modifiers that make up the binding.
	std::vector<Input::ModifierKey> Modifiers{};

	/// @brief Input mode (hold or release).
	InputMode Mode;

	/// @brief Input device (mouse or keyboard).
	InputDevice Device;

	Input::RegularKey GetKey(int index) const { return Keys[index]; }

	Input::ModifierKey GetModifier(int index) const { return Modifiers[index]; }

	Input::MouseButton GetButton() const { return Button; }

	size_t GetKeyCount() const { return Keys.size(); }

	size_t GetModifierCount() const { return Modifiers.size(); }

	bool HasKeys() const { return !Keys.empty(); }

	bool HasModifiers() const { return !Modifiers.empty(); }

	bool HasMouseButton() const { return Button != Input::MouseButton::Unknown; }
};

/// @brief Represents an input event with its binding and associated action.
struct Event
{
	ActionType Action;
	InputBinding Binding;

	size_t Priority() const { return Binding.GetKeyCount() + Binding.GetModifierCount(); }
};

/// @brief Comparator for Event struct to sort by priority.
/// @note Events with more keys have higher priority.
struct EventComparator
{
	bool operator()(const Event& lhs, const Event& rhs) const { return lhs.Priority() >= rhs.Priority(); }
};

/// @brief Manages input events and actions.
class EventManager
{
public:
	using SectionStr = std::string;
	using DescriptionStr = std::string;
	using Descriptor = std::unordered_map<SectionStr, std::vector<std::pair<ActionType, DescriptionStr>>>;
	using ActionToBinding =
		std::unordered_map<ActionType, std::vector<InputBinding>, HashHelper::EnumStructHash<ActionType>>;
	using TimePoint = std::chrono::_V2::system_clock::time_point;

public:
	/// @brief Process all events and trigger actions accordingly.
	/// @param dt Delta time since last call.
	void HandleEvents(const double dt);

	/// @brief Register a new action with multiple keys.
	/// @param keys List of keys that trigger the action.
	/// @param modifiers List of modifiers that trigger the action.
	/// @param mode Input mode (hold or release).
	/// @param action Action to be triggered.
	void RegisterKeyboardAction(
		std::initializer_list<Input::RegularKey> keys,
		std::initializer_list<Input::ModifierKey> modifiers,
		const InputMode mode,
		const ActionType action);

	/// @brief Register a new mouse action with multiple buttons.
	/// @param button Mouse button that trigger the action.
	/// @param modifiers List of modifiers that trigger the action.
	/// @param mode Input mode (hold or release).
	/// @param action Action to be triggered.
	void RegisterMouseAction(
		Input::MouseButton button,
		std::initializer_list<Input::ModifierKey> modifiers,
		const InputMode mode,
		const ActionType action);

	/// @brief Set the action descriptor.
	/// @param descriptor Descriptor mapping sections to bindings and descriptions.
	void SetDescriptor(const Descriptor& descriptor);

	/// @brief Set the description for a specific action.
	/// @param action Action to set the description for.
	/// @param section Section name.
	/// @param description Description of the action.
	void SetDescription(const ActionType action, const std::string& section, const std::string& description);

	/// @brief Returns true if the key is pressed, false otherwise.
	bool IsKeyPressed(GLFWwindow* window, Input::RegularKey key) const;

	/// @return Returns true if any event is running, false otherwise.
	bool HasEventRunning() const;

	/// @brief Returns true if the layout is QWERTY, false otherwise.
	bool IsAzertyLayout() const;

	/// @brief Toggles the keyboard layout between AZERTY and QWERTY.
	void ToggleKeyboardLayout();

private:
	/// @brief Register a new action event.
	/// @param binding Input binding that triggers the action.
	/// @param action ActionType to be triggered.
	void RegisterActionEvents(const InputBinding& binding, ActionType action);

protected:
	/// @brief Handle key events.
	/// @note Will most likely be called in a callback from the windowing system.
	void OnKeyEvent(int key, int scancode, int action, int mods);

	/// @brief Handle mouse button events.
	/// @note Will most likely be called in a callback from the windowing system.
	void OnMouseButtonEvent(int button, int action, int mods);

	/// @brief Handle cursor movement events.
	/// @note Will most likely be called in a callback from the windowing system.
	void OnCursorEvent(double xPos, double yPos, int windowWidth, int windowHeight);

	/// @brief Handle scroll events.
	/// @note Will most likely be called in a callback from the windowing system.
	void OnScrollEvent(double xOffset, double yOffset);

	/// @brief Called when an action starts.
	virtual void OnActionStart() = 0;

	/// @brief Called when an action is executed.
	virtual void OnActionExecute() = 0;

	/// @brief Called when an action ends.
	virtual void OnActionEnd() = 0;

	/// @brief Handle scroll event.
	/// @note Is called from HandleEvents.
	/// @param dt Delta time since last call.
	virtual void HandleScrollEvent(const double dt) = 0;

	/// @brief Handle double click event.
	/// @param button Mouse button that was double clicked.
	virtual void HandleDoubleClickEvent(const Input::MouseButton button) = 0;

	/// @brief Render the ImGui descriptor for the event manager.
	virtual void RenderImGuiDescriptor() const = 0;

private:
	/// @brief ActionType descriptor mapping sections to bindings and descriptions.
	Descriptor m_ActionTypeDescriptor{};

	/// @brief Map of pressed inputs to track their state.
	std::unordered_map<Input::InputSource, bool> m_InputPressed{};

	/// @brief Map of held inputs to track their state.
	std::unordered_map<Input::InputSource, bool> m_InputHeld{};

	/// @brief Map of consumed inputs to prevent multiple actions from triggering on the same key press.
	std::unordered_map<Input::InputSource, bool> m_InputConsumed{};

	/// @brief Map of actions that have started.
	std::unordered_map<ActionType, bool> m_StartedEvent{};

	/// @brief Set of registered events sorted by priority.
	/// @note Events with more keys have higher priority.
	std::set<Event, EventComparator> m_RegisteredEvents{};

	/// @brief Mapping from actions to their input bindings.
	ActionToBinding m_ActionToBindMapping{};

	/// @brief Keyboard layout (AZERTY or QWERTY).
	Input::KeyboardLayout m_KeybordLayout{ Input::KeyboardLayout::Qwerty };

	/// @brief Time point of the last mouse click.
	TimePoint m_LastTimeClick{ std::chrono::high_resolution_clock::now() };

	/// @brief Last mouse button that was clicked.
	Input::MouseButton m_LastButtonClicked{ Input::MouseButton::Unknown };

	/// @brief Current mouse position.
	std::pair<float, float> m_MousePosition{};

	/// @brief Mouse movement delta since last event.
	std::pair<float, float> m_MouseDelta{};

	/// @brief Mouse movement delta for roll calculations.
	std::pair<float, float> m_RollMouseDelta{};

	/// @brief Scroll Y offset.
	double m_YOffset{ 0 };
};

} // namespace Core

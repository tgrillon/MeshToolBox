#pragma once

#include "Core/Event/Event.h"
#include "Core/Event/MouseCodes.h"

namespace Core
{
/// @brief Mouse moved event class.
class MouseMovedEvent : public Event
{
public:
	MouseMovedEvent(const float x, const float y)
		: m_MouseX(x)
		, m_MouseY(y)
	{}

	float GetX() const { return m_MouseX; }

	float GetY() const { return m_MouseY; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseMoved)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
	float m_MouseX;
	float m_MouseY;
};

/// @brief Mouse scrolled event class.
class MouseScrolledEvent : public Event
{
public:
	MouseScrolledEvent(const float xOffset, const float yOffset)
		: m_XOffset(xOffset)
		, m_YOffset(yOffset)
	{}

	float GetXOffset() const { return m_XOffset; }

	float GetYOffset() const { return m_YOffset; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseScrolled)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
	float m_XOffset;
	float m_YOffset;
};

/// @brief Base class for mouse button events.
class MouseButtonEvent : public Event
{
public:
	MouseCode GetMouseButton() const { return m_Button; }

	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
protected:
	MouseButtonEvent(const MouseCode button)
		: m_Button(button)
	{}

	/// @brief The mouse button associated with the event.
	MouseCode m_Button;
};

/// @brief Mouse button pressed event class.
class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
	MouseButtonPressedEvent(const MouseCode button)
		: MouseButtonEvent(button)
	{}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << m_Button;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonPressed)
};

/// @brief Mouse button released event class.
class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
	MouseButtonReleasedEvent(const MouseCode button)
		: MouseButtonEvent(button)
	{}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << m_Button;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonReleased)
};

} // namespace Core

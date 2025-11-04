#pragma once

#include "Core/Event/Event.h"

namespace Core
{

/// @brief Event class for window resize events.
class WindowResizeEvent : public Event
{
public:
	/// @brief Constructs a WindowResizeEvent with the given width and height.
	WindowResizeEvent(unsigned int width, unsigned int height)
		: m_Width(width)
		, m_Height(height)
	{}

	/// @brief Gets the new width of the window.
	unsigned int GetWidth() const { return m_Width; }

	/// @brief Gets the new height of the window.
	unsigned int GetHeight() const { return m_Height; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
		return ss.str();
	}

	EVENT_CLASS_TYPE(WindowResize)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
private:
	unsigned int m_Width;
	unsigned int m_Height;
};

/// @brief Event class for window close events.
class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() = default;

	EVENT_CLASS_TYPE(WindowClose)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
};
} // namespace Core

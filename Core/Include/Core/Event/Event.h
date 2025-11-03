#pragma once

#include <sstream>
#include <string>

#define BIT(x) (1 << x)

namespace Core
{

/// @brief Different types of events.
enum class EventType
{
	None = 0,
	WindowClose,
	WindowResize,
	WindowFocus,
	WindowLostFocus,
	WindowMoved,

	KeyPressed,
	KeyReleased,
	KeyTyped,

	MouseButtonPressed,
	MouseButtonReleased,
	MouseMoved,
	MouseScrolled,
	MouseDoubleClick
};

/// @brief Categories for events, used for filtering.
enum EventCategory
{
	None = 0,
	EventCategoryApplication = BIT(0), // 0000
	EventCategoryInput = BIT(1), // 0001
	EventCategoryKeyboard = BIT(2), // 0010
	EventCategoryMouse = BIT(3), // 0100
	EventCategoryMouseButton = BIT(4) // 1000
};

#define EVENT_CLASS_TYPE(type) \
	static EventType GetStaticType() \
	{ \
		return EventType::type; \
	} \
	virtual EventType GetEventType() const override \
	{ \
		return GetStaticType(); \
	} \
	virtual const char* GetName() const override \
	{ \
		return #type; \
	}

#define EVENT_CLASS_CATEGORY(category) \
	virtual int GetCategoryFlags() const override \
	{ \
		return category; \
	}

/// @brief Base class for all events.
class Event
{
public:
	/// @brief Virtual destructor.
	virtual ~Event() = default;

	/// @brief Indicates whether the event has been handled.
	bool Handled = false;

	/// @brief Gets the type of the event.
	virtual EventType GetEventType() const = 0;
	/// @brief Gets the name of the event.
	virtual const char* GetName() const = 0;
	/// @brief Gets the category flags of the event.
	virtual int GetCategoryFlags() const = 0;

	/// @brief Converts the event to a string representation.
	virtual std::string ToString() const { return GetName(); }

	/// @brief Checks if the event is in a specific category.
	bool IsInCategory(EventCategory category) { return GetCategoryFlags() & category; }
};

/// @brief Dispatches events to the appropriate handler based on their type.
class EventDispatcher
{
public:
	/// @brief Constructs an EventDispatcher for a given event.
	explicit EventDispatcher(Event& event)
		: m_Event(event)
	{}

	/// @brief Dispatches the event to the provided function if the event type matches.
	/// @tparam T The type of the event to dispatch.
	/// @tparam F The type of the function to call.
	/// @param func The function to call if the event type matches.
	/// @return True if the event was dispatched, false otherwise.
	template<typename T, typename F>
	bool Dispatch(const F& func)
	{
		if(m_Event.GetEventType() == T::GetStaticType())
		{
			m_Event.Handled |= func(static_cast<T&>(m_Event));
			return true;
		}
		return false;
	}

private:
	/// @brief The event to dispatch.
	Event& m_Event;
};

/// @brief Overloads the output stream operator for events.
inline std::ostream& operator<<(std::ostream& os, const Event& e)
{
	return os << e.ToString();
}

} // namespace Core

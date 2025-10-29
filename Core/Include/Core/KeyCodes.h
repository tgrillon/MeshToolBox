#pragma once

#include <bitset>
#include <cstdint>
#include <string>

namespace Core::Input
{

/// @brief Regular keys.
enum struct RegularKey : int16_t
{
	Unknown = -1,

	// From glfw3.h
	Space = 32,
	Apostrophe = 39, /* ' */
	Comma = 44, /* , */
	Minus = 45, /* - */
	Period = 46, /* . */
	Slash = 47, /* / */

	D0 = 48, /* 0 */
	D1 = 49, /* 1 */
	D2 = 50, /* 2 */
	D3 = 51, /* 3 */
	D4 = 52, /* 4 */
	D5 = 53, /* 5 */
	D6 = 54, /* 6 */
	D7 = 55, /* 7 */
	D8 = 56, /* 8 */
	D9 = 57, /* 9 */

	Semicolon = 59, /* ; */
	Equal = 61, /* = */

	A = 65,
	B = 66,
	C = 67,
	D = 68,
	E = 69,
	F = 70,
	G = 71,
	H = 72,
	I = 73,
	J = 74,
	K = 75,
	L = 76,
	M = 77,
	N = 78,
	O = 79,
	P = 80,
	Q = 81,
	R = 82,
	S = 83,
	T = 84,
	U = 85,
	V = 86,
	W = 87,
	X = 88,
	Y = 89,
	Z = 90,

	LeftBracket = 91, /* [ */
	Backslash = 92, /* \ */
	RightBracket = 93, /* ] */
	GraveAccent = 96, /* ` */

	World1 = 161, /* non-US #1 */
	World2 = 162, /* non-US #2 */

	/* Function keys */
	Escape = 256,
	Enter = 257,
	Tab = 258,
	Backspace = 259,
	Insert = 260,
	Delete = 261,
	Right = 262,
	Left = 263,
	Down = 264,
	Up = 265,
	PageUp = 266,
	PageDown = 267,
	Home = 268,
	End = 269,
	CapsLock = 280,
	ScrollLock = 281,
	NumLock = 282,
	PrintScreen = 283,
	Pause = 284,
	F1 = 290,
	F2 = 291,
	F3 = 292,
	F4 = 293,
	F5 = 294,
	F6 = 295,
	F7 = 296,
	F8 = 297,
	F9 = 298,
	F10 = 299,
	F11 = 300,
	F12 = 301,
	F13 = 302,
	F14 = 303,
	F15 = 304,
	F16 = 305,
	F17 = 306,
	F18 = 307,
	F19 = 308,
	F20 = 309,
	F21 = 310,
	F22 = 311,
	F23 = 312,
	F24 = 313,
	F25 = 314,

	/* Keypad */
	KP0 = 320,
	KP1 = 321,
	KP2 = 322,
	KP3 = 323,
	KP4 = 324,
	KP5 = 325,
	KP6 = 326,
	KP7 = 327,
	KP8 = 328,
	KP9 = 329,
	KPDecimal = 330,
	KPDivide = 331,
	KPMultiply = 332,
	KPSubtract = 333,
	KPAdd = 334,
	KPEnter = 335,
	KPEqual = 336,
};

/// @brief  @brief Modifier keys
enum struct ModifierKey : int16_t
{
	Unknown = -1,
	LeftShift = 340,
	LeftControl = 341,
	LeftAlt = 342,
	LeftSuper = 343,
	RightShift = 344,
	RightControl = 345,
	RightAlt = 346,
	RightSuper = 347,
	Menu = 348
};

/// @brief Key states.
enum struct KeyState : int8_t
{
	None = -1,
	Released,
	Pressed,
	Repeated,
	Held
};

/// @brief Cursor modes.
enum struct CursorMode : uint8_t
{
	Normal = 0,
	Hidden = 1,
	Locked = 2
};

/// @brief Mouse buttons.
enum struct MouseButton : int16_t
{
	Unknown = -1,

	Button0 = 0,
	Button1 = 1,
	Button2 = 2,
	Button3 = 3,
	Button4 = 4,
	Button5 = 5,
	Left = Button0,
	Right = Button1,
	Middle = Button2
};

/// @brief Input source representing either a regular key, modifier key, or mouse button.
struct InputSource
{
	enum struct InputType : uint8_t
	{
		RegularKeyType,
		ModifierKeyType,
		MouseButtonType
	};

	/// @brief Type of the input source.
	InputType Type;

	/// @brief The actual input value.
	union
	{
		RegularKey Key;
		ModifierKey Modifier;
		MouseButton Button;
	};

	/// @brief Equality operator for InputSource.
	bool operator==(const InputSource& other) const
	{
		if(Type != other.Type)
		{
			return false;
		}

		switch(Type)
		{
			case InputType::RegularKeyType:
				return Key == other.Key;
			case InputType::ModifierKeyType:
				return Modifier == other.Modifier;
			case InputType::MouseButtonType:
				return Button == other.Button;
		}

		return false; // Should never reach here
	}

	// Optional but recommended for C++20
	bool operator!=(const InputSource& other) const { return !(*this == other); }
};

enum struct KeyboardLayout
{
	Azerty,
	Qwerty
};

/// @brief Returns the corresponding key in the AZERTY layout.
inline RegularKey MapToAZERTY(const RegularKey key)
{
	switch(key)
	{
		case RegularKey::Q:
			return RegularKey::A;
		case RegularKey::A:
			return RegularKey::Q;
		case RegularKey::W:
			return RegularKey::Z;
		case RegularKey::Z:
			return RegularKey::W;
		case RegularKey::Semicolon:
			return RegularKey::M;
		case RegularKey::M:
			return RegularKey::Comma;
		default:
			return key;
	}
}

/// @brief Returns the corresponding key in the QWERTY layout.
inline RegularKey MapToQWERTY(const RegularKey key)
{
	switch(key)
	{
		case RegularKey::Q:
			return RegularKey::A;
		case RegularKey::A:
			return RegularKey::Q;
		case RegularKey::W:
			return RegularKey::Z;
		case RegularKey::Z:
			return RegularKey::W;
		case RegularKey::M:
			return RegularKey::Semicolon;
		case RegularKey::Comma:
			return RegularKey::M;
		default:
			return key;
	}
}

/// @brief Returns the string representation of a RegularKey.
inline std::string GetStrFromInput(RegularKey key)
{
	if(static_cast<uint16_t>(key) >= 39 && static_cast<uint16_t>(key) <= 96)
	{
		return std::string(1, static_cast<char>(key));
	}

	switch(key)
	{
		case RegularKey::Space:
			return "SPACE";
		case RegularKey::Escape:
			return "ESCAPE";
		case RegularKey::Enter:
			return "ENTER";
		case RegularKey::Tab:
			return "TAB";
		case RegularKey::Backspace:
			return "BACKSPACE";
		case RegularKey::Insert:
			return "INSERT";
		case RegularKey::Delete:
			return "DELETE";
		case RegularKey::Right:
			return "RIGHT";
		case RegularKey::Left:
			return "Left";
		case RegularKey::Down:
			return "Down";
		case RegularKey::Up:
			return "Up";
		case RegularKey::PageUp:
			return "PAGE_UP";
		case RegularKey::PageDown:
			return "PAGE_DOWN";
		case RegularKey::Home:
			return "HOME";
		case RegularKey::End:
			return "END";
		case RegularKey::CapsLock:
			return "CAPS_LOCK";
		case RegularKey::ScrollLock:
			return "SCROLL_LOCK";
		case RegularKey::NumLock:
			return "NUM_LOCK";
		case RegularKey::PrintScreen:
			return "PRINT_SCREEN";
		case RegularKey::Pause:
			return "PAUSE";
		case RegularKey::F1:
			return "F1";
		case RegularKey::F2:
			return "F2";
		case RegularKey::F3:
			return "F3";
		case RegularKey::F4:
			return "F4";
		case RegularKey::F5:
			return "F5";
		case RegularKey::F6:
			return "F6";
		case RegularKey::F7:
			return "F7";
		case RegularKey::F8:
			return "F8";
		case RegularKey::F9:
			return "F9";
		case RegularKey::F10:
			return "F10";
		case RegularKey::F11:
			return "F11";
		case RegularKey::F12:
			return "F12";
		case RegularKey::F13:
			return "F13";
		case RegularKey::F14:
			return "F14";
		case RegularKey::F15:
			return "F15";
		case RegularKey::F16:
			return "F16";
		case RegularKey::F17:
			return "F17";
		case RegularKey::F18:
			return "F18";
		case RegularKey::F19:
			return "F19";
		case RegularKey::F20:
			return "F20";
		case RegularKey::F21:
			return "F21";
		case RegularKey::F22:
			return "F22";
		case RegularKey::F23:
			return "F23";
		case RegularKey::F24:
			return "F24";
		case RegularKey::F25:
			return "F25";
		case RegularKey::KP0:
			return "NUMP_0";
		case RegularKey::KP1:
			return "NUMP_1";
		case RegularKey::KP2:
			return "NUMP_2";
		case RegularKey::KP3:
			return "NUMP_3";
		case RegularKey::KP4:
			return "NUMP_4";
		case RegularKey::KP5:
			return "NUMP_5";
		case RegularKey::KP6:
			return "NUMP_6";
		case RegularKey::KP7:
			return "NUMP_7";
		case RegularKey::KP8:
			return "NUMP_8";
		case RegularKey::KP9:
			return "NUMP_9";
		case RegularKey::KPDecimal:
			return "NUMP_DECIMAL";
		case RegularKey::KPDivide:
			return "NUMP_DIVIDE";
		case RegularKey::KPMultiply:
			return "NUMP_MULTIPLY";
		case RegularKey::KPSubtract:
			return "NUMP_SUBTRACT";
		case RegularKey::KPAdd:
			return "NUMP_ADD";
		case RegularKey::KPEnter:
			return "NUMP_ENTER";
		case RegularKey::KPEqual:
			return "NUMP_EQUAL";
		default:
			return "UNKNOWN";
	}
}

/// @brief Returns the string representation of a ModifierKey.
inline std::string GetStrFromInput(ModifierKey key)
{
	switch(key)
	{
		case ModifierKey::LeftShift:
			return "LSHIFT";
		case ModifierKey::LeftControl:
			return "LCTRL";
		case ModifierKey::LeftAlt:
			return "LALT";
		case ModifierKey::LeftSuper:
			return "LSUPER";
		case ModifierKey::RightShift:
			return "RSHIFT";
		case ModifierKey::RightControl:
			return "RCTRL";
		case ModifierKey::RightAlt:
			return "RALT";
		case ModifierKey::RightSuper:
			return "RSUPER";
		case ModifierKey::Menu:
			return "MENU";
		default:
			return "UNKNOWN";
	}
}

/// @brief Returns the string representation of a MouseButton.
inline std::string GetStrFromInput(MouseButton button)
{
	switch(button)
	{
		case MouseButton::Button3:
			return "MOUSE_BTN_3";
		case MouseButton::Button4:
			return "MOUSE_BTN_4";
		case MouseButton::Button5:
			return "MOUSE_BTN_5";
		case MouseButton::Left:
			return "MOUSE_BTN_LEFT";
		case MouseButton::Right:
			return "MOUSE_BTN_RIGHT";
		case MouseButton::Middle:
			return "MOUSE_BTN_MIDDLE";
		default:
			return "UNKNOWN";
	}
}

} // namespace Core::Input

namespace std
{
template<>
struct hash<Core::Input::InputSource>
{
	size_t operator()(Core::Input::InputSource input) const
	{
		switch(input.Type)
		{
			case Core::Input::InputSource::InputType::RegularKeyType:
				return static_cast<size_t>(input.Key);
			case Core::Input::InputSource::InputType::ModifierKeyType:
				return static_cast<size_t>(input.Modifier);
			case Core::Input::InputSource::InputType::MouseButtonType:
				return static_cast<size_t>(input.Button);
			default:
				return 0;
		}
	}
};
} // namespace std

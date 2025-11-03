#pragma once

#include <bitset>
#include <cstdint>
#include <string>

namespace Core::Input
{

/// @brief Key codes (From glfw3.h).
enum struct KeyCode : int16_t
{
	Unknown = -1,

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

/// @brief Returns the corresponding key in the QWERTY layout.
inline KeyCode MapToQWERTY(const KeyCode key)
{
	switch(key)
	{
		case KeyCode::Q:
			return KeyCode::A;
		case KeyCode::A:
			return KeyCode::Q;
		case KeyCode::W:
			return KeyCode::Z;
		case KeyCode::Z:
			return KeyCode::W;
		case KeyCode::M:
			return KeyCode::Semicolon;
		case KeyCode::Comma:
			return KeyCode::M;
		default:
			return key;
	}
}

/// @brief Returns the string representation of a KeyCode.
inline std::string GetStrFromInput(KeyCode key)
{
	if(static_cast<uint16_t>(key) >= 39 && static_cast<uint16_t>(key) <= 96)
	{
		return std::string(1, static_cast<char>(key));
	}

	switch(key)
	{
		case KeyCode::Space:
			return "SPACE";
		case KeyCode::Escape:
			return "ESCAPE";
		case KeyCode::Enter:
			return "ENTER";
		case KeyCode::Tab:
			return "TAB";
		case KeyCode::Backspace:
			return "BACKSPACE";
		case KeyCode::Insert:
			return "INSERT";
		case KeyCode::Delete:
			return "DELETE";
		case KeyCode::Right:
			return "RIGHT";
		case KeyCode::Left:
			return "Left";
		case KeyCode::Down:
			return "Down";
		case KeyCode::Up:
			return "Up";
		case KeyCode::PageUp:
			return "PAGE_UP";
		case KeyCode::PageDown:
			return "PAGE_DOWN";
		case KeyCode::Home:
			return "HOME";
		case KeyCode::End:
			return "END";
		case KeyCode::CapsLock:
			return "CAPS_LOCK";
		case KeyCode::ScrollLock:
			return "SCROLL_LOCK";
		case KeyCode::NumLock:
			return "NUM_LOCK";
		case KeyCode::PrintScreen:
			return "PRINT_SCREEN";
		case KeyCode::Pause:
			return "PAUSE";
		case KeyCode::F1:
			return "F1";
		case KeyCode::F2:
			return "F2";
		case KeyCode::F3:
			return "F3";
		case KeyCode::F4:
			return "F4";
		case KeyCode::F5:
			return "F5";
		case KeyCode::F6:
			return "F6";
		case KeyCode::F7:
			return "F7";
		case KeyCode::F8:
			return "F8";
		case KeyCode::F9:
			return "F9";
		case KeyCode::F10:
			return "F10";
		case KeyCode::F11:
			return "F11";
		case KeyCode::F12:
			return "F12";
		case KeyCode::F13:
			return "F13";
		case KeyCode::F14:
			return "F14";
		case KeyCode::F15:
			return "F15";
		case KeyCode::F16:
			return "F16";
		case KeyCode::F17:
			return "F17";
		case KeyCode::F18:
			return "F18";
		case KeyCode::F19:
			return "F19";
		case KeyCode::F20:
			return "F20";
		case KeyCode::F21:
			return "F21";
		case KeyCode::F22:
			return "F22";
		case KeyCode::F23:
			return "F23";
		case KeyCode::F24:
			return "F24";
		case KeyCode::F25:
			return "F25";
		case KeyCode::KP0:
			return "NUMP_0";
		case KeyCode::KP1:
			return "NUMP_1";
		case KeyCode::KP2:
			return "NUMP_2";
		case KeyCode::KP3:
			return "NUMP_3";
		case KeyCode::KP4:
			return "NUMP_4";
		case KeyCode::KP5:
			return "NUMP_5";
		case KeyCode::KP6:
			return "NUMP_6";
		case KeyCode::KP7:
			return "NUMP_7";
		case KeyCode::KP8:
			return "NUMP_8";
		case KeyCode::KP9:
			return "NUMP_9";
		case KeyCode::KPDecimal:
			return "NUMP_DECIMAL";
		case KeyCode::KPDivide:
			return "NUMP_DIVIDE";
		case KeyCode::KPMultiply:
			return "NUMP_MULTIPLY";
		case KeyCode::KPSubtract:
			return "NUMP_SUBTRACT";
		case KeyCode::KPAdd:
			return "NUMP_ADD";
		case KeyCode::KPEnter:
			return "NUMP_ENTER";
		case KeyCode::KPEqual:
			return "NUMP_EQUAL";
		case KeyCode::LeftShift:
			return "LSHIFT";
		case KeyCode::LeftControl:
			return "LCTRL";
		case KeyCode::LeftAlt:
			return "LALT";
		case KeyCode::LeftSuper:
			return "LSUPER";
		case KeyCode::RightShift:
			return "RSHIFT";
		case KeyCode::RightControl:
			return "RCTRL";
		case KeyCode::RightAlt:
			return "RALT";
		case KeyCode::RightSuper:
			return "RSUPER";
		case KeyCode::Menu:
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

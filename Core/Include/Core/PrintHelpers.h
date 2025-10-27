#pragma once
#include <format>
#include <iostream>
#include <source_location>

namespace PrintHelpers
{
// ANSI color codes for terminal output
namespace Color
{
constexpr const char* Reset = "\033[0m";
constexpr const char* Red = "\033[31m";
constexpr const char* Green = "\033[32m";
constexpr const char* Yellow = "\033[33m";
constexpr const char* Blue = "\033[34m";
constexpr const char* Magenta = "\033[35m";
constexpr const char* Cyan = "\033[36m";
constexpr const char* Gray = "\033[90m";
constexpr const char* BrightRed = "\033[91m";
} // namespace Color

// Helper to extract just the function name from the full signature
inline std::string_view ExtractFunctionName(std::string_view signature)
{
	// Find the opening parenthesis (marks end of function name)
	auto paren_pos = signature.find('(');
	if(paren_pos == std::string_view::npos)
		return signature;

	// Work backwards from the parenthesis to find the start of the function name
	// Skip spaces before the parenthesis
	while(paren_pos > 0 && signature[paren_pos - 1] == ' ')
		--paren_pos;

	auto name_end = paren_pos;

	// Find the start of the function name
	auto name_start = signature.rfind(' ', name_end);

	// Take whichever delimiter comes last
	if(name_start != std::string_view::npos)
	{
		name_start = name_start + 1; // +1 to skip the space
	}
	else
	{
		name_start = 0;
	}

	return signature.substr(name_start, name_end - name_start);
}

// Basic output functions
template<typename... Args>
void Print(std::format_string<Args...> fmt, Args&&... args)
{
	(std::cout) << std::format(fmt, std::forward<Args>(args)...) << '\n';
}

template<typename... Args>
void PrintErr(std::format_string<Args...> fmt, Args&&... args)
{
	std::cerr << std::format(fmt, std::forward<Args>(args)...) << '\n';
}

// Implementation functions (not called directly)
namespace detail
{
template<typename... Args>
void InfoImpl(const std::source_location& loc, std::format_string<Args...> fmt, Args&&... args)
{
	auto func_name = ExtractFunctionName(loc.function_name());
	std::cout << std::format(
		"{}[INFO]{} {}[{}()]{} {}\n",
		Color::Cyan,
		Color::Reset,
		Color::Blue,
		func_name,
		Color::Reset,
		std::format(fmt, std::forward<Args>(args)...));
}

template<typename... Args>
void SuccessImpl(const std::source_location& loc, std::format_string<Args...> fmt, Args&&... args)
{
	auto func_name = ExtractFunctionName(loc.function_name());
	std::cout << std::format(
		"{}[SUCCESS]{} {}[{}()]{} {}\n",
		Color::Green,
		Color::Reset,
		Color::Blue,
		func_name,
		Color::Reset,
		std::format(fmt, std::forward<Args>(args)...));
}

template<typename... Args>
void WarningImpl(const std::source_location& loc, std::format_string<Args...> fmt, Args&&... args)
{
	auto func_name = ExtractFunctionName(loc.function_name());
	std::cerr << std::format(
		"{}[WARNING]{} {}[{}()]{} {}\n",
		Color::Yellow,
		Color::Reset,
		Color::Blue,
		func_name,
		Color::Reset,
		std::format(fmt, std::forward<Args>(args)...));
}

template<typename... Args>
void ErrorImpl(const std::source_location& loc, std::format_string<Args...> fmt, Args&&... args)
{
	auto func_name = ExtractFunctionName(loc.function_name());
	std::cerr << std::format(
		"{}[ERROR]{} {}[{}()]{} {}\n",
		Color::BrightRed,
		Color::Reset,
		Color::Blue,
		func_name,
		Color::Reset,
		std::format(fmt, std::forward<Args>(args)...));
}

template<typename... Args>
void DebugImpl(const std::source_location& loc, std::format_string<Args...> fmt, Args&&... args)
{
#ifdef NDEBUG
	(void)sizeof...(args);
#else
	auto func_name = ExtractFunctionName(loc.function_name());
	std::cout << std::format(
		"{}[DEBUG]{} {}[{}()]{} {}\n",
		Color::Gray,
		Color::Reset,
		Color::Blue,
		func_name,
		Color::Reset,
		std::format(fmt, std::forward<Args>(args)...));
#endif
}

template<typename... Args>
void TraceImpl(const std::source_location& loc, std::format_string<Args...> fmt, Args&&... args)
{
#ifdef NDEBUG
	(void)sizeof...(args);
#else
	auto func_name = ExtractFunctionName(loc.function_name());
	std::cout << std::format(
		"{}[TRACE]{} {}[{}()]{} {}\n",
		Color::Magenta,
		Color::Reset,
		Color::Blue,
		func_name,
		Color::Reset,
		std::format(fmt, std::forward<Args>(args)...));
#endif
}
} // namespace detail

} // namespace PrintHelpers

// Macros to capture the caller's location
#define Info(...) PrintHelpers::detail::InfoImpl(std::source_location::current(), __VA_ARGS__)

#define Success(...) PrintHelpers::detail::SuccessImpl(std::source_location::current(), __VA_ARGS__)

#define Warning(...) PrintHelpers::detail::WarningImpl(std::source_location::current(), __VA_ARGS__)

#define Error(...) PrintHelpers::detail::ErrorImpl(std::source_location::current(), __VA_ARGS__)

#define Debug(...) PrintHelpers::detail::DebugImpl(std::source_location::current(), __VA_ARGS__)

#define Trace(...) PrintHelpers::detail::TraceImpl(std::source_location::current(), __VA_ARGS__)

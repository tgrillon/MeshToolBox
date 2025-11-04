#pragma once

#include "Core/BaseTypes.h"
#include "Core/Event/Event.h"

#include <GLFW/glfw3.h>

#include <functional>
#include <string>

namespace Core
{

struct WindowSpecification
{
	std::string Title;
	uint32_t Width = 1280;
	uint32_t Height = 720;
	bool IsResizeable = true;
	bool VSync = true;
};

class Window
{
public:
	using EventCallbackFn = std::function<void(Event&)>;

public:
	explicit Window(const WindowSpecification& specification = WindowSpecification());
	~Window();

	void Update();

	BaseType::Vec2 GetFramebufferSize();

	bool ShouldClose() const;

	GLFWwindow* GetHandle() const { return m_Handle; }

	void SetEventCallback(const EventCallbackFn& callback);

private:
	WindowSpecification m_Specification;

	GLFWwindow* m_Handle = nullptr;

	struct WindowData
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		bool IsResizeable;
		bool VSync;

		EventCallbackFn EventCallback;
	};

	WindowData m_Data;
};

} // namespace Core

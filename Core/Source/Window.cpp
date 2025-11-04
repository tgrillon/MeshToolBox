#include "Core/Window.h"

#include "Core/Event/ApplicationEvent.h"
#include "Core/Event/KeyEvents.h"

#include <assert.h>
#include <glad/gl.h>

#include <iostream>

namespace Core
{

Window::Window(const WindowSpecification& specification)
	: m_Specification(specification)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	m_Data.Width = m_Specification.Width;
	m_Data.Height = m_Specification.Height;
	m_Data.VSync = m_Specification.VSync;
	m_Data.IsResizeable = m_Specification.IsResizeable;

	m_Handle = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);

	if(!m_Handle)
	{
		std::cerr << "Failed to create GLFW window!\n";
		assert(false);
	}

	glfwMakeContextCurrent(m_Handle);
	gladLoadGL(glfwGetProcAddress);

	glfwSetWindowUserPointer(m_Handle, &m_Data);
	glfwSwapInterval(m_Data.VSync ? 1 : 0);

	// Set GLFW callbacks
	glfwSetWindowSizeCallback(
		m_Handle,
		[](GLFWwindow* window, int width, int height)
		{
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			data->Width = width;
			data->Height = height;

			WindowResizeEvent event(width, height);
			data->EventCallback(event);
		});

	glfwSetWindowCloseCallback(
		m_Handle,
		[](GLFWwindow* window)
		{
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			WindowCloseEvent event;
			data->EventCallback(event);
		});

	glfwSetKeyCallback(
		m_Handle,
		[](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch(action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data->EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data->EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, true);
					data->EventCallback(event);
					break;
				}
			}
		});
}

Window::~Window()
{
	if(m_Handle)
		glfwDestroyWindow(m_Handle);

	m_Handle = nullptr;
}

void Window::Update()
{
	glfwSwapBuffers(m_Handle);
}

glm::vec2 Window::GetFramebufferSize()
{
	int width, height;
	glfwGetFramebufferSize(m_Handle, &width, &height);
	return { width, height };
}

bool Window::ShouldClose() const
{
	return glfwWindowShouldClose(m_Handle) != 0;
}

void Window::SetEventCallback(const EventCallbackFn& callback)
{
	m_Data.EventCallback = callback;
}

} // namespace Core

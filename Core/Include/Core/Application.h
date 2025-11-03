#pragma once

#include "Core/BaseTypes.h"
#include "Core/Layer.h"
#include "Core/Window.h"

#include <functional>
#include <memory>
#include <set>
#include <string>
#include <vector>

namespace Core
{

struct ApplicationSpecification
{
	std::string Name = "Application";
	WindowSpecification WindowSpec;
};

class Application
{
public:
	explicit Application(const ApplicationSpecification& specification = ApplicationSpecification());
	~Application();

	void Run();
	void Stop();

	template<typename TLayer>
		requires(std::is_base_of_v<Layer, TLayer>)
	void PushLayer()
	{
		m_LayerStack.push_back(std::make_unique<TLayer>());
	}

	BaseType::Vec2 GetFramebufferSize() const;
	GLFWwindow* GetWindow() const;

	bool IsDarkModeEnabled() const;
	bool& DisplayMode();

	void SetMenubarCallback(const std::function<void()>& funCallback) { m_MenubarCallback = funCallback; }

	static Application& Get();
	static float GetTime();

private:
	ApplicationSpecification m_Specification;
	std::shared_ptr<Window> m_Window;
	bool m_Running = false;

	std::function<void()> m_MenubarCallback;
	std::vector<std::unique_ptr<Layer>> m_LayerStack;
};

// Implemented by CLIENT
std::unique_ptr<Application> CreateApplication(int argc, char** argv);
} // namespace Core

#include "Core/Application.h"

#include "Core/Event/Input.h"
#include "Core/Event/KeyCodes.h"
#include "Core/PrintHelpers.h"
#include "Core/Renderer/GLUtils.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <cassert>
#include <iostream>

extern bool g_ApplicationRunning;

namespace Core
{

using namespace BaseType;

static Application* s_Application = nullptr;

static void GLFWErrorCallback(int error, const char* description)
{
	Error("[GLFW Error]: {}", description);
}

Application::Application(const ApplicationSpecification& specification)
	: m_Specification(specification)
{
	s_Application = this;

	glfwSetErrorCallback(GLFWErrorCallback);
	glfwInit();

	// Set window title to app name if empty
	if(m_Specification.WindowSpec.Title.empty())
		m_Specification.WindowSpec.Title = m_Specification.Name;

	m_Window = std::make_shared<Window>(m_Specification.WindowSpec);

	Renderer::Utils::InitOpenGLDebugMessageCallback();

	// ImGui initialization
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	IM_UNUSED(io);
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	// ImGui style
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL(m_Window->GetHandle(), true);
	ImGui_ImplOpenGL3_Init("#version 460");

	m_Window->SetEventCallback(
		[this](Event& event)
		{
			this->OnEvent(event);
		});
}

Application::~Application()
{
	// ImGui clean up
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	m_LayerStack.clear();

	m_Window->Destroy();
	glfwTerminate();

	s_Application = nullptr;

	g_ApplicationRunning = false;
}

void Application::Run()
{
	m_Running = true;

	float lastTime = GetTime();

	// Main Application loop
	while(m_Running)
	{
		glfwPollEvents();

		if(m_Window->ShouldClose())
		{
			Stop();
			break;
		}

		float currentTime = GetTime();
		float timestep = glm::clamp(currentTime - lastTime, 0.001f, 0.1f);
		lastTime = currentTime;

		// Main layer update here
		for(const std::unique_ptr<Layer>& layer : m_LayerStack)
			layer->OnUpdate(timestep);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
			// because it would be confusing to have two docking targets within each others.
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
			if(m_MenubarCallback)
				window_flags |= ImGuiWindowFlags_MenuBar;

			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
				| ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

			// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
			// and handle the pass-thru hole, so we ask Begin() to not render a background.
			if(dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
			// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
			// all active windows docked into it will lose their parent and become undocked.
			// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
			// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace Demo", nullptr, window_flags);
			ImGui::PopStyleVar();

			ImGui::PopStyleVar(2);

			// Submit the DockSpace
			ImGuiIO& io = ImGui::GetIO();
			if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("VulkanAppDockspace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			if(m_MenubarCallback)
			{
				if(ImGui::BeginMenuBar())
				{
					m_MenubarCallback();
					ImGui::EndMenuBar();
				}
			}

			// NOTE: rendering can be done elsewhere (eg. render thread)
			for(const std::unique_ptr<Layer>& layer : m_LayerStack)
				layer->OnRender();

			ImGui::End();
		}

		ImGui::Render();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGuiIO& io = ImGui::GetIO();
		IM_UNUSED(io);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		m_Window->Update();
	}
}

void Application::Stop()
{
	m_Running = false;
}

Vec2 Application::GetFramebufferSize() const
{
	return m_Window->GetFramebufferSize();
}

Application& Application::Get()
{
	assert(s_Application);
	return *s_Application;
}

GLFWwindow* Application::GetWindow()
{
	return s_Application->m_Window->GetHandle();
}

float Application::GetTime()
{
	return (float)glfwGetTime();
}

void Application::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<WindowResizeEvent>(
		[this](WindowResizeEvent& event)
		{
			return this->OnResizeEvent(event);
		});
	dispatcher.Dispatch<WindowCloseEvent>(
		[this](WindowCloseEvent& event)
		{
			return this->OnCloseEvent(event);
		});
	dispatcher.Dispatch<KeyReleasedEvent>(
		[this](KeyReleasedEvent& event)
		{
			return this->OnKeyReleasedEvent(event);
		});

	for(const std::unique_ptr<Layer>& layer : m_LayerStack)
	{
		if(event.Handled)
			break;
		layer->OnEvent(event);
	}
}

bool Application::OnResizeEvent(WindowResizeEvent&)
{
	return true;
}

bool Application::OnCloseEvent(WindowCloseEvent&)
{
	m_Running = false;
	return true;
}

bool Application::OnKeyReleasedEvent(KeyReleasedEvent& event)
{
	if(Input::IsKeyReleased(Key::Escape))
	{
		m_Running = false;
		return true;
	}

	return false;
}
} // namespace Core

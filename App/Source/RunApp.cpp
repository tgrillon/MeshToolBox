#include "Application/AppLayer.h"
#include "Core/Application.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

bool g_ApplicationRunning = true;

namespace
{
std::unique_ptr<Core::Application> CreateApplication(int argc, char** argv)
{
	Core::ApplicationSpecification appSpec;
	appSpec.Name = (argc > 1) ? argv[1] : "My Dear Application";
	appSpec.WindowSpec.Width = (argc > 2) ? std::stoi(argv[2]) : 800;
	appSpec.WindowSpec.Height = (argc > 3) ? std::stoi(argv[3]) : 500;

	auto app = std::make_unique<Core::Application>(appSpec);
	app->PushLayer<App::AppLayer>();
	app->SetMenubarCallback(
		[&app]()
		{
			if(ImGui::BeginMenu("File"))
			{
				if(ImGui::MenuItem("Exit"))
				{
					app->Stop();
				}
				ImGui::EndMenu();
			}
		});

	return app;
}
} // namespace

namespace Application
{

int Run(int argc, char** argv)
{
	while(g_ApplicationRunning)
	{
		std::unique_ptr<Core::Application> myApp = CreateApplication(argc, argv);
		myApp->Run();
	}

	return 0;
}
} // namespace Application

#include "Application/AppLayer.h"

#include "Core/Application.h"
#include "Core/PrintHelpers.h"
#include "Core/Renderer/Buffer.h"
#include "Core/Renderer/Shader.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glm/glm.hpp>

using namespace Core;
using namespace Core::BaseType;

namespace Application
{
AppLayer::AppLayer()
{
	// Create shaders
	m_Shader = Renderer::CreateGraphicsShader("Data/Shaders/BaseVertex.glsl", "Data/Shaders/BaseFragment.glsl");

	// Create geometry
	glCreateVertexArrays(VaoType::NbVao, m_Vao);
	glCreateBuffers(VboType::NbVbo, m_Vbo);

	float p = 1.0;
	float n = 0.0;

	std::vector<float> positions = {
		n, p, n, /* */ p, p, n, /* */ p, p, p, // tri1 dessus
		n, p, n, /* */ p, p, p, /* */ n, p, p, // tri2

		n, n, n, /* */ p, n, n, /* */ p, p, n, // tri1 avant
		n, n, n, /* */ p, p, n, /* */ n, p, n, // tri2

		p, n, n, /* */ p, n, p, /* */ p, p, p, // tri1 droite
		p, n, n, /* */ p, p, p, /* */ p, p, n, // tri2

		p, n, p, /* */ n, n, p, /* */ n, p, p, // tri1 arriÃ¨re
		p, n, p, /* */ n, p, p, /* */ p, p, p, // tri2

		n, n, p, /* */ n, n, n, /* */ n, p, n, // tri1 gauche
		n, n, p, /* */ n, p, n, /* */ n, p, p, // tri2

		n, n, n, /* */ p, n, n, /* */ p, n, p, // tri1 dessous
		n, n, n, /* */ p, n, p, /* */ n, n, p, // tri2
	};

	std::vector<float> texCoords = {
		1 / 18.f, 1 / 2.f, /* */ 2 / 18.f, 1 / 2.f, /* */ 2 / 18.f, 3 / 4.f, // up
		1 / 18.f, 1 / 2.f, /* */ 2 / 18.f, 3 / 4.f, /* */ 1 / 18.f, 3 / 4.f, // up

		1 / 18.f, 1 / 4.f, /* */ 2 / 18.f, 1 / 4.f, /* */ 2 / 18.f, 1 / 2.f, // front
		1 / 18.f, 1 / 4.f, /* */ 2 / 18.f, 1 / 2.f, /* */ 1 / 18.f, 1 / 2.f, // front

		1 / 6.f,  1 / 2.f, /* */ 1 / 6.f,  3 / 4.f, /* */ 2 / 18.f, 3 / 4.f, // right
		1 / 6.f,  1 / 2.f, /* */ 2 / 18.f, 3 / 4.f, /* */ 2 / 18.f, 1 / 2.f, // right

		2 / 18.f, 1.f,	   /* */ 1 / 18.f, 1.f,		/* */ 1 / 18.f, 3 / 4.f, // back
		2 / 18.f, 1.f,	   /* */ 1 / 18.f, 3 / 4.f, /* */ 2 / 18.f, 3 / 4.f, // back

		0.f,	  3 / 4.f, /* */ 0.f,	   1 / 2.f, /* */ 1 / 18.f, 1 / 2.f, // left
		0.f,	  3 / 4.f, /* */ 1 / 18.f, 1 / 2.f, /* */ 1 / 18.f, 3 / 4.f, // left

		1 / 18.f, 0.0f,	   /* */ 2 / 18.f, 0.0f,	/* */ 2 / 18.f, 1 / 4.f, // bottom
		1 / 18.f, 0.0f,	   /* */ 2 / 18.f, 1 / 4.f, /* */ 1 / 18.f, 1 / 4.f, // bottom
	};

	std::vector<float> normals = {
		0,	1,	0,	/* */ 0,  1,  0,  /* */ 0,	1,	0, // up
		0,	1,	0,	/* */ 0,  1,  0,  /* */ 0,	1,	0, // up

		0,	0,	-1, /* */ 0,  0,  -1, /* */ 0,	0,	-1, // front
		0,	0,	-1, /* */ 0,  0,  -1, /* */ 0,	0,	-1, // front

		1,	0,	0,	/* */ 1,  0,  0,  /* */ 1,	0,	0, // right
		1,	0,	0,	/* */ 1,  0,  0,  /* */ 1,	0,	0, // right

		0,	0,	1,	/* */ 0,  0,  1,  /* */ 0,	0,	1, // back
		0,	0,	1,	/* */ 0,  0,  1,  /* */ 0,	0,	1, // back

		-1, 0,	0,	/* */ -1, 0,  0,  /* */ -1, 0,	0, // left
		-1, 0,	0,	/* */ -1, 0,  0,  /* */ -1, 0,	0, // left

		0,	-1, 0,	/* */ 0,  -1, 0,  /* */ 0,	-1, 0, // bottom
		0,	-1, 0,	/* */ 0,  -1, 0,  /* */ 0,	-1, 0, // bottom
	};

	glBindVertexArray(m_Vao[VaoType::Object]);
	Renderer::LoadBuffer(m_Vbo[VboType::Position], VboType::Position, 3, positions);
	Renderer::LoadBuffer(m_Vbo[VboType::TexCoords], VboType::TexCoords, 2, texCoords);
	Renderer::LoadBuffer(m_Vbo[VboType::Normal], VboType::Normal, 3, normals);

	m_Texture = Renderer::LoadTexture("Data/Texture/debug2x2red.png");

	glBindTexture(GL_TEXTURE_2D, m_Texture.Handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	m_Camera.SetLookAt(Vec3{ 0, 0, 0 });
	Vec2 fbSize = Core::Application::Get().GetFramebufferSize();
	m_Camera.SetViewport(0, 0, fbSize.x, fbSize.y);

	// Print various OpenGL informations to stdout
	Info(
		"{} {}",
		reinterpret_cast<const char*>(glGetString(GL_VENDOR)),
		reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
	Info("GLFW \t {}", glfwGetVersionString());
	Info("OpenGL\t {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	Info("GLSL\t {}", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
}

AppLayer::~AppLayer()
{
	glDeleteVertexArrays(VaoType::NbVao, m_Vao);
	glDeleteBuffers(VboType::NbVbo, m_Vbo);

	glDeleteProgram(m_Shader);

	static Renderer::Texture renderTexture;
	static Renderer::Framebuffer renderFramebuffer;

	if(renderTexture.Handle != 0)
	{
		glDeleteTextures(1, &renderTexture.Handle);
		glDeleteFramebuffers(1, &renderFramebuffer.Handle);
	}

	if(m_Texture.Handle != 0)
	{
		glDeleteTextures(1, &m_Texture.Handle);
	}
}

void AppLayer::OnUpdate(float)
{}

void AppLayer::OnRender()
{
	// ImGui stuff
	ImGui::Begin("My Application");

	const float windowWidth = ImGui::GetContentRegionAvail().x;
	const float windowHeight = ImGui::GetContentRegionAvail().y;

	if(windowWidth <= 0 || windowHeight <= 0)
	{
		ImGui::End();
		return;
	}

	static Renderer::Texture renderTexture;
	static Renderer::Framebuffer renderFramebuffer;

	if(renderTexture.Width != windowWidth || renderTexture.Height != windowHeight)
	{
		// Clean up old resources
		if(renderTexture.Handle != 0)
		{
			glDeleteTextures(1, &renderTexture.Handle);
			glDeleteFramebuffers(1, &renderFramebuffer.Handle);
		}

		// Create new texture and framebuffer
		renderTexture = Renderer::CreateTexture(windowWidth, windowHeight);
		renderFramebuffer = Renderer::CreateFramebufferWithTexture(renderTexture);
	}

	ImVec2 pos = ImGui::GetCursorScreenPos();

	ImGui::GetWindowDrawList()->AddImage(
		reinterpret_cast<void*>(static_cast<uintptr_t>(renderTexture.Handle)),
		ImVec2(pos.x, pos.y),
		ImVec2(pos.x + windowWidth, pos.y + windowHeight),
		ImVec2(0, 1),
		ImVec2(1, 0));
	ImGui::End();

	// Drawing stuff
	glBindFramebuffer(GL_FRAMEBUFFER, renderFramebuffer.Handle);
	glViewport(0, 0, static_cast<GLsizei>(windowWidth), static_cast<GLsizei>(windowHeight));
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	double relativeCursorX = 0.0;
	double relativeCursorY = 0.0;
	glfwGetCursorPos(Core::Application::GetWindow(), &relativeCursorX, &relativeCursorY);

	Mat4 p, v, m;
	m_Camera.GetMatricies(p, v, m);

	Mat4 mvp = p * v * m;

	glUseProgram(m_Shader);
	glUniformMatrix4fv(Renderer::GetUniformLocation("u_Mvp", m_Shader), 1, GL_TRUE, glm::value_ptr(mvp));
	glUniform1i(Renderer::GetUniformLocation("u_Texture", m_Shader), 0);

	// Render
	glBindVertexArray(m_Vao[VaoType::Object]);
	glDrawArrays(GL_TRIANGLES, 0, 8);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void AppLayer::OnEvent(Event&)
{}

} // namespace Application

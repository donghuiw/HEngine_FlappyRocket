#include "GameLayer.h"
#include "HEngine/Debug/Instrumentor.h"
#include "HEngine/Scene/SceneSerializer.h"
#include "HEngine/Utils/PlatformUtils.h"
#include "HEngine/Math/Math.h"
#include "HEngine/Scripting/ScriptEngine.h"
#include "HEngine/Renderer/Font.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace HEngine
{
	GameLayer::GameLayer()
		: Layer("GameLayer")
	{
		auto& window = Application::Get().GetWindow();
		CreateCamera(window.GetWidth(), window.GetHeight());

		Random::Init();
	}

	void GameLayer::OnAttach()
	{
		m_Scene = CreateRef<Scene>();

		auto commandLineArgs = Application::Get().GetSpecification().CommandLineArgs;
		if (commandLineArgs.Count > 1)
		{
			auto projectFilePath = commandLineArgs[1];
			OpenProject(projectFilePath);
		}
		else
		{
			if (!OpenProject())
				Application::Get().Close();
		}

		m_Level.Init();

		ImGuiIO io = ImGui::GetIO();
		m_Font = io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Regular.ttf", 120.0f);
	}

	void GameLayer::OnDetach()
	{
	}

	void GameLayer::OnUpdate(Timestep ts)
	{
		m_Time += ts;
		if ((int)(m_Time * 10.0f) % 8 > 4)
			m_Blink = !m_Blink;

		if (m_Level.IsGameOver())
			m_State = GameState::GameOver;

		const auto& playerPos = m_Level.GetPlayer().GetPosition();
		m_Camera->SetPosition({ playerPos.x, playerPos.y, 0.0f });

		switch (m_State)
		{
		case GameState::Play:
		{
			m_Level.OnUpdate(ts);
			break;
		}
		}

		// Render
		Renderer2D::ResetStats();
		RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1 });
		RenderCommand::Clear();

		HEngine::Renderer2D::BeginScene(*m_Camera);
		m_Level.OnRender();
		HEngine::Renderer2D::EndScene();
	}

	void GameLayer::OnImGuiRender()
	{
		//ImGui::Begin("Settings");
		//m_Level.OnImGuiRender();
		//ImGui::End();

		// UI?

		switch (m_State)
		{
		case GameState::Play:
		{
			uint32_t playerScore = m_Level.GetPlayer().GetScore();
			std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
			ImGui::GetForegroundDrawList()->AddText(m_Font, 48.0f, ImGui::GetWindowPos(), 0xffffffff, scoreStr.c_str());
			break;
		}
		case GameState::MainMenu:
		{
			auto pos = ImGui::GetWindowPos();
			auto width = Application::Get().GetWindow().GetWidth();
			auto height = Application::Get().GetWindow().GetHeight();
			pos.x += width * 0.5f - 300.0f;
			pos.y += 50.0f;
			if (m_Blink)
				ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xffffffff, "Click to Play!");
			break;
		}
		case GameState::GameOver:
		{
			auto pos = ImGui::GetWindowPos();
			auto width = Application::Get().GetWindow().GetWidth();
			auto height = Application::Get().GetWindow().GetHeight();
			pos.x += width * 0.5f - 300.0f;
			pos.y += 50.0f;
			if (m_Blink)
				ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xffffffff, "Click to Play!");

			pos.x += 200.0f;
			pos.y += 150.0f;
			uint32_t playerScore = m_Level.GetPlayer().GetScore();
			std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
			ImGui::GetForegroundDrawList()->AddText(m_Font, 48.0f, pos, 0xffffffff, scoreStr.c_str());
			break;
		}
		}
	}

	bool GameLayer::OpenProject()
	{
		std::string filepath = FileDialogs::OpenFile("HEngine Project (*.hproj)\0*.hproj\0");
		if (filepath.empty())
			return false;

		OpenProject(filepath);
		return true;
	}

	void GameLayer::OpenProject(const std::filesystem::path& path)
	{
		if (Project::Load(path))
		{
			ScriptEngine::Init();

			auto startScenePath = Project::GetAssetFileSystemPath(Project::GetActive()->GetConfig().StartScene);
			OpenScene(startScenePath);

		}
	}

	void GameLayer::OpenScene(const std::filesystem::path& path)
	{
		if (path.extension().string() != ".hengine")
		{
			HE_WARN("Could not load {0} - not a scene file", path.filename().string());
			return;
		}

		Ref<Scene> newScene = CreateRef<Scene>();
		SceneSerializer serializer(newScene);
		if (serializer.Deserialize(path.string()))
		{
			m_Scene = newScene;
		}
	}

	void GameLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(HE_BIND_EVENT_FN(GameLayer::OnWindowResize));
		dispatcher.Dispatch<MouseButtonPressedEvent>(HE_BIND_EVENT_FN(GameLayer::OnMouseButtonPressed));
	}

	bool GameLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (m_State == GameState::GameOver)
			m_Level.Reset();

		m_State = GameState::Play;
		return false;
	}

	bool GameLayer::OnWindowResize(WindowResizeEvent& e)
	{
		CreateCamera(e.GetWidth(), e.GetHeight());
		return false;
	}

	void GameLayer::CreateCamera(uint32_t width, uint32_t height)
	{
		float aspectRatio = (float)width / (float)height;

		float camWidth = 8.0f;
		float bottom = -camWidth;
		float top = camWidth;
		float left = bottom * aspectRatio;
		float right = top * aspectRatio;
		m_Camera = CreateScope<OrthographicCamera>(left, right, bottom, top);
	}
}
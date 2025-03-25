#pragma once

#include "HEngine.h"
#include "Game/Level.h"
#include "HEngine/Scene/Scene.h"

#include <imgui/imgui.h>

namespace HEngine
{
	class GameLayer : public HEngine::Layer
	{
	public:
		GameLayer();
		virtual ~GameLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(HEngine::Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(HEngine::Event& e) override;
		bool OnMouseButtonPressed(HEngine::MouseButtonPressedEvent& e);
		bool OnWindowResize(HEngine::WindowResizeEvent& e);

	private:
		void CreateCamera(uint32_t width, uint32_t height);
		void OpenProject(const std::filesystem::path& path);
		void OpenScene(const std::filesystem::path& path);
		bool OpenProject();
	private:
		Scope<HEngine::OrthographicCamera> m_Camera;
		Level m_Level;
		ImFont* m_Font;
		float m_Time = 0.0f;
		bool m_Blink = false;

		Ref<Scene> m_Scene;
		enum class GameState
		{
			Play = 0, MainMenu = 1, GameOver = 2
		};

		GameState m_State = GameState::MainMenu;
	};
}
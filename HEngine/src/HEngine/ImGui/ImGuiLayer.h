#pragma once

#include "HEngine/Core/Layer.h"

#include "HEngine/Events/ApplicationEvent.h"
#include "HEngine/Events/KeyEvent.h"
#include "HEngine/Events/MouseEvent.h"

namespace HEngine
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }

		void SetDarkThemeColors();

		uint32_t GetActiveWidgetID() const;
	private:
		bool m_BlockEvents = true;
	};
}

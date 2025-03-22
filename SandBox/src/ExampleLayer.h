#pragma once

#include "HEngine.h"

class ExampleLayer : public HEngine::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(HEngine::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(HEngine::Event& e) override;
private:
	HEngine::ShaderLibrary m_ShaderLibrary;
	HEngine::Ref<HEngine::Shader> m_Shader;
	HEngine::Ref<HEngine::VertexArray> m_VertexArray;

	HEngine::Ref<HEngine::Shader> m_FlatColorShader;
	HEngine::Ref<HEngine::VertexArray> m_SquareVA;

	HEngine::Ref<HEngine::Texture2D> m_Texture, m_ChernoLogoTexture;

	HEngine::OrthographicCameraController m_CameraController;
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

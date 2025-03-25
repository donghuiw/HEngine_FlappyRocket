#pragma once
#include <HEngine.h>

#include "Color.h"
#include "Random.h"

#include "ParticleSystem.h"

class Player
{
public:
	Player();

	void LoadAssets();

	void OnUpdate(HEngine::Timestep ts);
	void OnRender();

	void OnImGuiRender();

	void Reset();

	float GetRotation() { return m_Velocity.y * 4.0f - 90.0f; }
	const glm::vec2& GetPosition() const { return m_Position; }

	uint32_t GetScore() const { return (uint32_t)(m_Position.x + 10.0f) / 10.0f; }
private:
	glm::vec2 m_Position = { -10.0f, 0.0f };
	glm::vec2 m_Velocity = { 5.0f, 0.0f };

	float m_EnginePower = 0.5f;	//控制力
	float m_Gravity = 0.4f;		//重力

	float m_Time = 0.0f;
	float m_SmokeEmitInterval = 0.4f;		//发射时间间隔
	float m_SmokeNextEmitTime = m_SmokeEmitInterval;		//下次发射时间

	ParticleProps m_SmokeParticle, m_EngineParticle;
	ParticleSystem m_PatricleSystem;

	HEngine::Ref<HEngine::Texture2D> m_ShipTexture;
};
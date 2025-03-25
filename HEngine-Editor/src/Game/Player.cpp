#include "Player.h"

#include <imgui/imgui.h>
#include "glm/gtc/matrix_transform.hpp"

using namespace HEngine;

Player::Player()
{
	// 初始化烟雾粒子参数
	m_SmokeParticle.Position = { 0.0f, 0.0f };
	m_SmokeParticle.Velocity = { -2.0f, 0.0f };  // 初始速度
	m_SmokeParticle.VelocityVariation = { 4.0f, 2.0f };  // 速度变化范围
	m_SmokeParticle.SizeBegin = 0.35f;
	m_SmokeParticle.SizeEnd = 0.0f;
	m_SmokeParticle.SizeVariation = 0.15f;
	m_SmokeParticle.ColorBegin = { 0.8f, 0.8f, 0.8f, 1.0f };  // 灰色烟雾
	m_SmokeParticle.ColorEnd = { 0.6f, 0.6f, 0.6f, 1.0f };
	m_SmokeParticle.LifeTime = 4.0f;  // 烟雾存活时间

	// 初始化火焰粒子参数
	m_EngineParticle.Position = { 0.0f, 0.0f };
	m_EngineParticle.Velocity = { -2.0f, 0.0f };
	m_EngineParticle.VelocityVariation = { 3.0f, 1.0f };
	m_EngineParticle.SizeBegin = 0.5f;
	m_EngineParticle.SizeEnd = 0.0f;
	m_EngineParticle.SizeVariation = 0.3f;
	m_EngineParticle.ColorBegin = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f }; // 橙色火焰
	m_EngineParticle.ColorEnd = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f , 1.0f }; // 黄色火焰
	m_EngineParticle.LifeTime = 1.0f; // 火焰消失得更快
}

void Player::LoadAssets()
{
	m_ShipTexture = Texture2D::Create("FlappyRocket/Assets/Textures/PinkRocket.png");
}

void Player::OnUpdate(HEngine::Timestep ts)
{
	m_Time += ts;		//记录时间

	if (Input::IsKeyPressed(Key::Space))	//如果按下空格（向上飞）
	{
		m_Velocity.y += m_EnginePower;	
		if (m_Velocity.y < 0.0f)
			m_Velocity.y += m_EnginePower * 2.0f;

		//计算火焰粒子的发射位置
		glm::vec2 emissionPoint = { 0.0f, -0.6f }; //火焰碰触的初始相对位置
		float rotation = glm::radians(GetRotation()); //获取火箭旋转角度
		glm::vec4 rotated = glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) * glm::vec4(emissionPoint, 0.0f, 1.0f);

		m_EngineParticle.Position = m_Position + glm::vec2{ rotated.x, rotated.y };
		m_EngineParticle.Velocity.y = -m_Velocity.y * 0.2f - 0.2f;	//反方向火焰效果
		m_PatricleSystem.Emit(m_EngineParticle);		//生成火焰粒子
	}
	else
	{
		m_Velocity.y -= m_Gravity;		//施加重力
	}

	m_Velocity.y = glm::clamp(m_Velocity.y, -20.0f, 20.0f);
	m_Position += m_Velocity * (float)ts;		//计算新位置

	//生成烟雾粒子
	if (m_Time > m_SmokeNextEmitTime)
	{
		m_SmokeParticle.Position = m_Position;		//烟雾跟随火箭
		m_PatricleSystem.Emit(m_SmokeParticle);		//生成烟雾粒子
		m_SmokeNextEmitTime += m_SmokeEmitInterval;	//更新下次生成烟雾的时间
	}

	//更新粒子
	m_PatricleSystem.OnUpdate(ts);
}

void Player::OnRender()
{
	m_PatricleSystem.OnRender();		//渲染粒子
	Renderer2D::DrawRotatedQuad({ m_Position.x, m_Position.y, 0.5f }, { 1.0f, 1.3f }, GetRotation(), m_ShipTexture);
}

void Player::OnImGuiRender()
{
	ImGui::DragFloat("Engine Power", &m_EnginePower, 0.1f);
	ImGui::DragFloat("Gravity", &m_Gravity, 0.1f);
}

void Player::Reset()
{
	m_Position = { -10.0f, 0.0f };
	m_Velocity = { 5.0f, 0.0f };
}

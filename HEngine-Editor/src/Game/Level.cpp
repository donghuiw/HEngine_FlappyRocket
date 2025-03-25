#include "Level.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace HEngine;

static glm::vec4 HSVtoRGB(const glm::vec3& hsv) {
	float H = fmod(hsv.x * 360.0f, 360.0f);  // 限制色相范围
	float S = hsv.y;
	float V = hsv.z;

	float C = S * V;  // 计算最大颜色通道的强度
	float X = C * (1.0f - fabs(fmod(H / 60.0f, 2) - 1.0f));
	float m = V - C;  // 计算最低颜色值
	glm::vec3 rgb;

	if (H < 60)      rgb = { C, X, 0 };
	else if (H < 120) rgb = { X, C, 0 };
	else if (H < 180) rgb = { 0, C, X };
	else if (H < 240) rgb = { 0, X, C };
	else if (H < 300) rgb = { X, 0, C };
	else              rgb = { C, 0, X };

	return { rgb.r + m, rgb.g + m, rgb.b + m, 1.0f }; // 透明度 1.0
}

static bool PointInTri(const glm::vec2& p, const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2) {
	float s1 = p2.y - p0.y;
	float s2 = p2.x - p0.x;
	float s3 = p1.y - p0.y;
	float s4 = p.y - p0.y;

	float w1 = (p0.x * s1 + s4 * s2 - p.x * s1) / (s3 * s2 - (p1.x - p0.x) * s1);
	float w2 = (s4 - w1 * s3) / s1;

	return (w1 >= 0) && (w2 >= 0) && (w1 + w2 <= 1);
}

void Level::Init()
{
		m_TriangleTexture = Texture2D::Create("FlappyRocket/Assets/Textures/Triangle.png");  // 载入三角形贴图
		m_Player.LoadAssets();	// 载入玩家资源

		m_Pillars.resize(5);			// 初始化 5 个柱子
		for (int i = 0; i < 5; i++)
			CreatePillar(i, i * 10.0f);		// 生成柱子
}

void Level::OnUpdate(HEngine::Timestep ts) {
	m_Player.OnUpdate(ts);  // 更新玩家状态

	if (CollisionTest()) {   // 碰撞检测
		GameOver();
		return;
	}

	// HSV 色相循环递增
	m_PillarHSV.x += 0.1f * ts;
	if (m_PillarHSV.x > 1.0f)
		m_PillarHSV.x = 0.0f;

	// 当玩家越过目标位置，创建新柱子
	if (m_Player.GetPosition().x > m_PillarTarget) {
		CreatePillar(m_PillarIndex, m_PillarTarget + 20.0f);
		m_PillarIndex = ++m_PillarIndex % m_Pillars.size();
		m_PillarTarget += 10.0f;
	}
}

void Level::OnRender()
{
	const auto& playerPos = m_Player.GetPosition();

	glm::vec4 color = HSVtoRGB(m_PillarHSV);

	// Background
	Renderer2D::DrawQuad({ playerPos.x, 0.0f, -0.8f }, { 50.0f, 50.0f }, { 0.3f, 0.3f, 0.3f, 1.0f });

	// Floor and ceiling
	Renderer2D::DrawQuad({ playerPos.x,  34.0f }, { 50.0f, 50.0f }, color);
	Renderer2D::DrawQuad({ playerPos.x, -34.0f }, { 50.0f, 50.0f }, color);

	for (auto& pillar : m_Pillars)
	{
		Renderer2D::DrawRotatedQuad(pillar.TopPosition, pillar.TopScale, 180.0f, m_TriangleTexture,1.0f, color);
		Renderer2D::DrawRotatedQuad(pillar.BottomPosition, pillar.BottomScale, 0.0f, m_TriangleTexture, 1.0f, color);
	}

	m_Player.OnRender();
}

void Level::OnImGuiRender()
{
	m_Player.OnImGuiRender();
}

void Level::CreatePillar(int index, float offset)
{
	Pillar& pillar = m_Pillars[index];
	pillar.TopPosition.x = offset;
	pillar.BottomPosition.x = offset;
	pillar.TopPosition.z = index * 0.1f - 0.5f;
	pillar.BottomPosition.z = index * 0.1f - 0.5f + 0.05f;

	float center = Random::Float(-15.0f, 15.0f); // 限制范围
	float gap = glm::mix(3.0f, 5.0f, Random::Float()); // 控制范围更合理

	pillar.TopPosition.y = 10.0f - ((10.0f - center) * 0.2f) + gap * 0.5f;
	pillar.BottomPosition.y = -10.0f - ((-10.0f - center) * 0.2f) - gap * 0.5f;
}

bool Level::CollisionTest()
{
	if (glm::abs(m_Player.GetPosition().y) > 8.5f)
		return true;

	glm::vec4 playerVertices[4] = {
		{ -0.5f, -0.5f, 0.0f, 1.0f },
		{  0.5f, -0.5f, 0.0f, 1.0f },
		{  0.5f,  0.5f, 0.0f, 1.0f },
		{ -0.5f,  0.5f, 0.0f, 1.0f }
	};

	const auto& pos = m_Player.GetPosition();
	glm::vec4 playerTransformedVerts[4];
	for (int i = 0; i < 4; i++)
	{
		playerTransformedVerts[i] = glm::translate(glm::mat4(1.0f), { pos.x, pos.y, 0.0f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_Player.GetRotation()), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { 1.0f, 1.3f, 1.0f })
			* playerVertices[i];
	}

	// To match Triangle.png (each corner is 10% from the texture edge)
	glm::vec4 pillarVertices[3] = {
		{ -0.5f + 0.1f, -0.5f + 0.1f, 0.0f, 1.0f },
		{  0.5f - 0.1f, -0.5f + 0.1f, 0.0f, 1.0f },
		{  0.0f + 0.0f,  0.5f - 0.1f, 0.0f, 1.0f },
	};

	for (auto& p : m_Pillars)
	{
		glm::vec2 tri[3];

		// Top pillars
		for (int i = 0; i < 3; i++)
		{
			tri[i] = glm::translate(glm::mat4(1.0f), { p.TopPosition.x, p.TopPosition.y, 0.0f })
				* glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), { 0.0f, 0.0f, 1.0f })
				* glm::scale(glm::mat4(1.0f), { p.TopScale.x, p.TopScale.y, 1.0f })
				* pillarVertices[i];
		}

		for (auto& vert : playerTransformedVerts)
		{
			if (PointInTri({ vert.x, vert.y }, tri[0], tri[1], tri[2]))
				return true;
		}

		// Bottom pillars
		for (int i = 0; i < 3; i++)
		{
			tri[i] = glm::translate(glm::mat4(1.0f), { p.BottomPosition.x, p.BottomPosition.y, 0.0f })
				* glm::scale(glm::mat4(1.0f), { p.BottomScale.x, p.BottomScale.y, 1.0f })
				* pillarVertices[i];
		}

		for (auto& vert : playerTransformedVerts)
		{
			if (PointInTri({ vert.x, vert.y }, tri[0], tri[1], tri[2]))
				return true;
		}

	}
	return false;
}

void Level::GameOver()
{
	m_GameOver = true;
}

void Level::Reset()
{
	m_GameOver = false;

	m_Player.Reset();

	m_PillarTarget = 30.0f;
	m_PillarIndex = 0;
	for (int i = 0; i < 5; i++)
		CreatePillar(i, i * 10.0f);
}
#pragma once

#include <HEngine.h>

struct ParticleProps
{
	glm::vec2 Position;
	glm::vec2 Velocity, VelocityVariation;		//初始速度&变化
	glm::vec4 ColorBegin, ColorEnd;				//初始 && 结束颜色
	float SizeBegin, SizeEnd, SizeVariation; //尺寸
	float LifeTime = 1.0f;	//生命周期
};

class ParticleSystem
{
public:
	ParticleSystem();

	void Emit(const ParticleProps& particleProps);
	void OnUpdate(HEngine::Timestep ts);
	void OnRender();

private:
	struct Particle
	{
		glm::vec2 Position;
		glm::vec2 Velocity;
		glm::vec4 ColorBegin, ColorEnd;
		float Rotation = 0.0f;
		float SizeBegin, SizeEnd;

		float LifeTime = 1.0f;
		float LifeRemaining = 0.0f;

		bool Active = false;
	};

	std::vector<Particle> m_ParticlePool;
	uint32_t m_PoolIndex = 999;
};
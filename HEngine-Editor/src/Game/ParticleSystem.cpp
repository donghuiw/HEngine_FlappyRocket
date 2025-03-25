#include "ParticleSystem.h"

#include "Random.h"

ParticleSystem::ParticleSystem()
{
	m_ParticlePool.resize(1000);	//Ԥ����1000������
}

void ParticleSystem::Emit(const ParticleProps& particleProps)
{
	for (size_t i = 0; i < m_ParticlePool.size(); i++)
	{
		m_PoolIndex = (m_PoolIndex - 1 + m_ParticlePool.size()) % m_ParticlePool.size();
		if (!m_ParticlePool[m_PoolIndex].Active)	//��ʹ����Ч����
			break;
	}

	Particle& particle = m_ParticlePool[m_PoolIndex];
	particle.Active = true;
	particle.Position = particleProps.Position;
	particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();		//�����ת

	//�ٶ�
	particle.Velocity = particleProps.Velocity;
	particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
	particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);

	//��ɫ
	particle.ColorBegin = particleProps.ColorBegin;
	particle.ColorEnd = particleProps.ColorEnd;

	//�ߴ�
	particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
	particle.SizeEnd = particleProps.SizeEnd;

	//����ʱ��
	particle.LifeTime = particleProps.LifeTime;
	particle.LifeRemaining = particleProps.LifeTime;

}

void ParticleSystem::OnUpdate(HEngine::Timestep ts)
{
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		if (particle.LifeRemaining <= 0.0f)
		{
			particle.Active = false;
			continue;
		}

		particle.LifeRemaining -= ts;
		particle.Position += particle.Velocity * (float)ts;
		particle.Rotation += 0.01f * ts;
	}
}

void ParticleSystem::OnRender()
{
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		float life = particle.LifeRemaining / particle.LifeTime;
		glm::vec4 color = glm::mix(particle.ColorEnd, particle.ColorBegin, life);
		color.a = color.a  * life;

		float size = glm::mix(particle.SizeEnd, particle.SizeBegin, life);
		HEngine::Renderer2D::DrawRotatedQuad(particle.Position, { size, size }, particle.Rotation, color);
	}
}


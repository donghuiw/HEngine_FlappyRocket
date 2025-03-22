#pragma once

#include <entt.hpp>

#include "HEngine/Scene/Components.h"

#include <box2d/id.h>
#include <box2d/box2d.h>
#include <box2d/collision.h>
#include <box2d/math_functions.h>

namespace HEngine
{
	namespace Utils
	{
		inline b2BodyType Rigidbody2DTypeToBox2DBody(Rigidbody2DComponent::BodyType bodyType)
		{
			switch (bodyType)
			{
				case HEngine::Rigidbody2DComponent::BodyType::Static:				return b2_staticBody;
				case HEngine::Rigidbody2DComponent::BodyType::Dynamic:			return b2_dynamicBody;
				case HEngine::Rigidbody2DComponent::BodyType::Kinematic:		return b2_kinematicBody;
			}
			HE_CORE_ASSERT(false, "Unknown body type");
			return b2_staticBody;
		}

		inline Rigidbody2DComponent::BodyType Rigidbody2DTypeFromBox2DBody(b2BodyType bodyType)
		{
			switch (bodyType)
			{
				case b2_staticBody:    return Rigidbody2DComponent::BodyType::Static;
				case b2_dynamicBody:   return Rigidbody2DComponent::BodyType::Dynamic;
				case b2_kinematicBody: return Rigidbody2DComponent::BodyType::Kinematic;
			}

			HE_CORE_ASSERT(false, "Unknown body type");
			return Rigidbody2DComponent::BodyType::Static;
		}
	}

	class Scene;
	class Timestep;
	struct Rigidbody2DComponent;
	struct BoxCollider2DComponent;

	class PhysicsManager
	{
	public:
		void CreateWorld();
		void DestoryWorld();
		void AddRigibody(Scene* scene, entt::entity e);
		void AttachBoxCollider(Scene* scene, entt::entity e);
		void AttachCircleCollider(Scene* scene, entt::entity e);
		void DestoryBoxCollider(Scene* scene, entt::entity e);
		void DestoryCircleCollider(Scene* scene, entt::entity e);
		void FixedUpdate(Timestep ts);
		void UpdateRigidbody(Scene* scnen, entt::entity e);
		bool ValidBody(b2BodyId bodyID);

	public:
		static PhysicsManager& Get() { return m_instance; }

	private:
		b2WorldId m_WorldId = b2_nullWorldId;

		static PhysicsManager m_instance;
	};
}
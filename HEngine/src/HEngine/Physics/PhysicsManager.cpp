#include "hepch.h"
#include "PhysicsManager.h"

#include "HEngine/Scene/ScriptableEntity.h"
#include "HEngine/Scene/Components.h"

namespace HEngine
{
	PhysicsManager PhysicsManager::m_instance;

	void PhysicsManager::CreateWorld()
	{
		b2WorldDef worldDef = b2DefaultWorldDef();
		worldDef.gravity = { 0.0f, -9.8f };
		worldDef.restitutionThreshold = 0.5f;
		m_WorldId = b2CreateWorld(&worldDef);
		HE_CORE_ASSERT(b2World_IsValid(m_WorldId),"World id validation failed. ");
	}

	void PhysicsManager::DestoryWorld()
	{
		b2DestroyWorld(m_WorldId);
		m_WorldId = b2_nullWorldId;
	}

	void PhysicsManager::AddRigibody(Scene* scene, entt::entity e)
	{
		Entity entity = { e, scene };

		if (!entity.HasComponent<Rigidbody2DComponent>()) return;

		auto& transform = entity.GetComponent<TransformComponent>();
		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

		if (b2Body_IsValid(rb2d.RuntimeBodyId)) return;

		b2BodyDef bodyDef = b2DefaultBodyDef();
		bodyDef.type = Utils::Rigidbody2DTypeToBox2DBody(rb2d.Type);
		bodyDef.position = { transform.Translation.x, transform.Translation.y };
		bodyDef.rotation = b2MakeRot(transform.Rotation.z);

		b2BodyId bodyId = b2CreateBody(m_WorldId, &bodyDef);
		b2Body_SetFixedRotation(bodyId, rb2d.FixedRotation);
		rb2d.RuntimeBodyId = bodyId;

		HE_CORE_ASSERT(b2Body_IsValid(rb2d.RuntimeBodyId), "Body id validation failed.");

		AttachBoxCollider(scene, e);
		AttachCircleCollider(scene, e);
	}

	void PhysicsManager::AttachBoxCollider(Scene* scene, entt::entity e)
	{
		Entity entity = { e, scene };

		if (!entity.HasComponent<BoxCollider2DComponent>() || !entity.HasComponent<Rigidbody2DComponent>()) return;

		auto& transform = entity.GetComponent<TransformComponent>();
		auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

		if (b2Shape_IsValid(bc2d.RuntimeShapeId)) return;
		if (!b2Body_IsValid(rb2d.RuntimeBodyId)) return;

		b2Polygon Polygon = b2MakeOffsetBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y, 
			{ bc2d.Offset.x, bc2d.Offset.y }, b2MakeRot(glm::radians(transform.Rotation.z)));

		b2ShapeDef shapeDef = b2DefaultShapeDef();
		shapeDef.friction = bc2d.Friction;
		shapeDef.density = bc2d.Density;
		shapeDef.restitution = bc2d.Restitution;

		b2ShapeId shapeID = b2CreatePolygonShape(rb2d.RuntimeBodyId, &shapeDef, &Polygon);
		bc2d.RuntimeShapeId = shapeID;

		HE_CORE_ASSERT(b2Shape_IsValid(bc2d.RuntimeShapeId), "Box Shape id validation failed.");
	}

	void PhysicsManager::AttachCircleCollider(Scene* scene, entt::entity e)
	{
		Entity entity = { e, scene };

		if (!entity.HasComponent<CircleCollider2DComponent>() || !entity.HasComponent<Rigidbody2DComponent>()) return;

		auto& transform = entity.GetComponent<TransformComponent>();
		auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

		if (b2Shape_IsValid(cc2d.RuntimeShapeId)) return;
		if (!b2Body_IsValid(rb2d.RuntimeBodyId)) return;

		b2Circle circleShape;
		circleShape.center = { cc2d.Offset.x, cc2d.Offset.y };
		circleShape.radius = transform.Scale.x *  cc2d.Radius;

		b2ShapeDef shapeDef = b2DefaultShapeDef();
		shapeDef.friction = cc2d.Friction;
		shapeDef.density = cc2d.Density;
		shapeDef.restitution = cc2d.Restitution;

		b2ShapeId shapeID = b2CreateCircleShape(rb2d.RuntimeBodyId, &shapeDef, &circleShape);
		cc2d.RuntimeShapeId = shapeID;

		HE_CORE_ASSERT(b2Shape_IsValid(cc2d.RuntimeShapeId), "Circle Shape id validation failed.");
	}

	void PhysicsManager::DestoryBoxCollider(Scene* scene, entt::entity e)
	{
		Entity entity = { e, scene };

		if (!entity.HasComponent<BoxCollider2DComponent>())return;

		auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
		if (!b2Shape_IsValid(bc2d.RuntimeShapeId)) return;
		
		b2DestroyShape(bc2d.RuntimeShapeId,false);
		bc2d.RuntimeShapeId = b2_nullShapeId;
	}

	void PhysicsManager::DestoryCircleCollider(Scene* scene, entt::entity e)
	{
		Entity entity = { e, scene };

		if (!entity.HasComponent<CircleCollider2DComponent>())return;

		auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
		if (!b2Shape_IsValid(cc2d.RuntimeShapeId)) return;

		b2DestroyShape(cc2d.RuntimeShapeId, false);
		cc2d.RuntimeShapeId = b2_nullShapeId;
	}

	void PhysicsManager::FixedUpdate(Timestep ts)
	{
		const int32_t subStepCount = 4;
		b2World_Step(m_WorldId, ts, subStepCount);
	}

	void PhysicsManager::UpdateRigidbody(Scene* scene, entt::entity e)
	{
		Entity entity = { e, scene };

		if (!entity.HasComponent<Rigidbody2DComponent>()) return;

		auto& transform = entity.GetComponent<TransformComponent>();
		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		if (!b2Body_IsValid(rb2d.RuntimeBodyId)) return;
		// Retrieve transform from Box2D
		
		b2Body_SetType(rb2d.RuntimeBodyId, Utils::Rigidbody2DTypeToBox2DBody(rb2d.Type));
		b2Body_SetFixedRotation(rb2d.RuntimeBodyId, rb2d.FixedRotation);

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			auto bc2d = entity.GetComponent<BoxCollider2DComponent>();
			if (b2Shape_IsValid(bc2d.RuntimeShapeId))
			{
				b2Polygon polygon = b2MakeOffsetBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y,
					{ bc2d.Offset.x, bc2d.Offset.y }, b2MakeRot(glm::radians(transform.Rotation.z)));

				b2Shape_SetPolygon(bc2d.RuntimeShapeId, &polygon);
				b2Shape_SetDensity(bc2d.RuntimeShapeId, bc2d.Density,true);
				b2Shape_SetFriction(bc2d.RuntimeShapeId, bc2d.Friction);
				b2Shape_SetRestitution(bc2d.RuntimeShapeId, bc2d.Restitution);
			}
		}

		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			auto cc2d = entity.GetComponent<CircleCollider2DComponent>();
			if (b2Shape_IsValid(cc2d.RuntimeShapeId))
			{
				b2Circle circleShape;
				circleShape.center = { cc2d.Offset.x, cc2d.Offset.y };
				circleShape.radius = transform.Scale.x * cc2d.Radius;

				b2Shape_SetCircle(cc2d.RuntimeShapeId, &circleShape);
				b2Shape_SetDensity(cc2d.RuntimeShapeId, cc2d.Density,true);
				b2Shape_SetFriction(cc2d.RuntimeShapeId, cc2d.Friction);
				b2Shape_SetRestitution(cc2d.RuntimeShapeId, cc2d.Restitution);
			}
		}
	}

	bool PhysicsManager::ValidBody(b2BodyId bodyID)
	{
		return b2Body_IsValid(bodyID);
	}

}
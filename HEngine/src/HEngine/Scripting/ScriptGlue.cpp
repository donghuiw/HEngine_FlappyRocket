#include "hepch.h"
#include "ScriptGlue.h"
#include "ScriptEngine.h"

#include "HEngine/Core/UUID.h"
#include "HEngine/Core/KeyCodes.h"
#include "HEngine/Core/Input.h"

#include "HEngine/Scene/Scene.h"
#include "HEngine/Scene/Entity.h"

#include "HEngine/Physics/PhysicsManager.h"

#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"

#include <box2d/box2d.h>

namespace HEngine
{
	namespace Utils
	{
		std::string MonoStringToString(MonoString* string)
		{
			char* cStr = mono_string_to_utf8(string);
			std::string str(cStr);
			mono_free(cStr);
			return str;
		}
	}
	
	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;

#define HE_ADD_INTERNAL_CALL(Name) mono_add_internal_call("HEngine.InternalCalls::" #Name, Name)

		static void NativeLog(MonoString* string, int parameter)
		{
			std::string str = Utils::MonoStringToString(string);
			std::cout << str << ", " << parameter << std::endl;
		}

		static MonoObject* GetScriptInstance(UUID entityID)
		{
			return ScriptEngine::GetManagedInstance(entityID);
		}

		static bool Entity_HasComponent(UUID entityID, MonoReflectionType* componentType)
		{
			Scene* scene = ScriptEngine::GetSceneContext();
			HE_CORE_ASSERT(scene);
			Entity entity = scene->GetEntityByUUID(entityID);
			HE_CORE_ASSERT(entity);

			MonoType* managedType = mono_reflection_type_get_type(componentType);
			HE_CORE_ASSERT(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end());
			return s_EntityHasComponentFuncs.at(managedType)(entity);
		}

		static uint64_t Entity_FindEntityByName(MonoString* name)
		{
			char* nameCStr = mono_string_to_utf8(name);

			Scene* scene = ScriptEngine::GetSceneContext();
			HE_CORE_ASSERT(scene);
			Entity entity = scene->FindEntityByName(nameCStr);
			mono_free(nameCStr);

			if (!entity)
				return 0;

			return entity.GetUUID();
		}
		static void TransformComponent_GetTranslation(UUID entityID, glm::vec3* outTranslation)
		{
			Scene* scene = ScriptEngine::GetSceneContext();
			HE_CORE_ASSERT(scene);
			Entity entity = scene->GetEntityByUUID(entityID);
			HE_CORE_ASSERT(entity);

			*outTranslation = entity.GetComponent<TransformComponent>().Translation;
		}

		static void TransformComponent_SetTranslation(UUID entityID, glm::vec3* translation)
		{
			Scene* scene = ScriptEngine::GetSceneContext();
			HE_CORE_ASSERT(scene);
			Entity entity = scene->GetEntityByUUID(entityID);
			HE_CORE_ASSERT(entity);

			entity.GetComponent<TransformComponent>().Translation = *translation;
		}

		static void Rigidbody2DComponent_ApplyLinearImpulse(UUID entityID, glm::vec2* impulse, glm::vec2* point, bool wake)
		{
			Scene* scene = ScriptEngine::GetSceneContext();
			HE_CORE_ASSERT(scene);
			Entity entity = scene->GetEntityByUUID(entityID);
			HE_CORE_ASSERT(entity);

			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
			b2BodyId bodyid = rb2d.RuntimeBodyId;
			b2Body_ApplyLinearImpulse(bodyid, b2Vec2{ impulse->x, impulse->y }, b2Vec2{ point->x, point->y }, wake);
		}

		static void Rigidbody2DComponent_ApplyLinearImpulseToCenter(UUID entityID, glm::vec2* impulse, bool wake)
		{
			Scene* scene = ScriptEngine::GetSceneContext();
			HE_CORE_ASSERT(scene);
			Entity entity = scene->GetEntityByUUID(entityID);
			HE_CORE_ASSERT(entity);

			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
			b2BodyId bodyid = rb2d.RuntimeBodyId;
			b2Body_ApplyLinearImpulseToCenter(bodyid, b2Vec2{ impulse->x, impulse->y }, wake);
		}

		static void Rigidbody2DComponent_GetLinearVelocity(UUID entityID, glm::vec2* outLinearVelocity)
		{
			Scene* scene = ScriptEngine::GetSceneContext();
			HE_CORE_ASSERT(scene);
			Entity entity = scene->GetEntityByUUID(entityID);
			HE_CORE_ASSERT(entity);

			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
			b2BodyId bodyId = rb2d.RuntimeBodyId;
			const b2Vec2 linearVelocity = b2Body_GetLinearVelocity(bodyId);
			*outLinearVelocity = glm::vec2(linearVelocity.x, linearVelocity.y);
		}

		static Rigidbody2DComponent::BodyType Rigidbody2DComponent_GetType(UUID entityID)
		{
			Scene* scene = ScriptEngine::GetSceneContext();
			HE_CORE_ASSERT(scene);
			Entity entity = scene->GetEntityByUUID(entityID);
			HE_CORE_ASSERT(entity);

			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
			b2BodyId bodyID = rb2d.RuntimeBodyId;
			return Utils::Rigidbody2DTypeFromBox2DBody(b2Body_GetType(bodyID));
		}

		static void Rigidbody2DComponent_SetType(UUID entityID, Rigidbody2DComponent::BodyType bodyType)
		{
			Scene* scene = ScriptEngine::GetSceneContext();
			HE_CORE_ASSERT(scene);
			Entity entity = scene->GetEntityByUUID(entityID);
			HE_CORE_ASSERT(entity);

			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
			b2BodyId bodyID = rb2d.RuntimeBodyId;
			b2Body_SetType(bodyID, Utils::Rigidbody2DTypeToBox2DBody(bodyType));
		}

		static MonoString* TextComponent_GetText(UUID entityID)
		{
			Scene* scene = ScriptEngine::GetSceneContext();
			HE_CORE_ASSERT(scene);
			Entity entity = scene->GetEntityByUUID(entityID);
			HE_CORE_ASSERT(entity);
			HE_CORE_ASSERT(entity.HasComponent<TextComponent>());

			auto& tc = entity.GetComponent<TextComponent>();
			return ScriptEngine::CreateString(tc.TextString.c_str());
		}

		static void TextComponent_SetText(UUID entityID, MonoString* textString)
		{
			Scene* scene = ScriptEngine::GetSceneContext();
			HE_CORE_ASSERT(scene);
			Entity entity = scene->GetEntityByUUID(entityID);
			HE_CORE_ASSERT(entity);
			HE_CORE_ASSERT(entity.HasComponent<TextComponent>());

			auto& tc = entity.GetComponent<TextComponent>();
			tc.TextString = Utils::MonoStringToString(textString);
		}

		static void TextComponent_GetColor(UUID entityID, glm::vec4* color)
		{
			Scene* scene = ScriptEngine::GetSceneContext();
			HE_CORE_ASSERT(scene);
			Entity entity = scene->GetEntityByUUID(entityID);
			HE_CORE_ASSERT(entity);
			HE_CORE_ASSERT(entity.HasComponent<TextComponent>());

			auto& tc = entity.GetComponent<TextComponent>();
			*color = tc.Color;
		}

		static void TextComponent_SetColor(UUID entityID, glm::vec4* color)
		{
			Scene* scene = ScriptEngine::GetSceneContext();
			HE_CORE_ASSERT(scene);
			Entity entity = scene->GetEntityByUUID(entityID);
			HE_CORE_ASSERT(entity);
			HE_CORE_ASSERT(entity.HasComponent<TextComponent>());

			auto& tc = entity.GetComponent<TextComponent>();
			tc.Color = *color;
		}

		static float TextComponent_GetKerning(UUID entityID)
		{
			Scene* scene = ScriptEngine::GetSceneContext();
			HE_CORE_ASSERT(scene);
			Entity entity = scene->GetEntityByUUID(entityID);
			HE_CORE_ASSERT(entity);
			HE_CORE_ASSERT(entity.HasComponent<TextComponent>());

			auto& tc = entity.GetComponent<TextComponent>();
			return tc.Kerning;
		}

		static void TextComponent_SetKerning(UUID entityID, float kerning)
		{
			Scene* scene = ScriptEngine::GetSceneContext();
			HE_CORE_ASSERT(scene);
			Entity entity = scene->GetEntityByUUID(entityID);
			HE_CORE_ASSERT(entity);
			HE_CORE_ASSERT(entity.HasComponent<TextComponent>());

			auto& tc = entity.GetComponent<TextComponent>();
			tc.Kerning = kerning;
		}

		static float TextComponent_GetLineSpacing(UUID entityID)
		{
			Scene* scene = ScriptEngine::GetSceneContext();
			HE_CORE_ASSERT(scene);
			Entity entity = scene->GetEntityByUUID(entityID);
			HE_CORE_ASSERT(entity);
			HE_CORE_ASSERT(entity.HasComponent<TextComponent>());

			auto& tc = entity.GetComponent<TextComponent>();
			return tc.LineSpacing;
		}

		static void TextComponent_SetLineSpacing(UUID entityID, float lineSpacing)
		{
			Scene* scene = ScriptEngine::GetSceneContext();
			HE_CORE_ASSERT(scene);
			Entity entity = scene->GetEntityByUUID(entityID);
			HE_CORE_ASSERT(entity);
			HE_CORE_ASSERT(entity.HasComponent<TextComponent>());

			auto& tc = entity.GetComponent<TextComponent>();
			tc.LineSpacing = lineSpacing;
		}


		static bool Input_IsKeyDown(KeyCode keycode)
		{
			return Input::IsKeyPressed(keycode);
		}

		template<typename... Component>
		static void RegisterComponent()
		{
			([]()
				{
					std::string_view typeName = typeid(Component).name();
					size_t pos = typeName.find_last_of(':');
					std::string_view structName = typeName.substr(pos + 1);
					std::string managedTypename = fmt::format("HEngine.{}", structName);

					MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(), ScriptEngine::GetCoreAssemblyImage());
					if (!managedType)
					{
						HE_CORE_WARN("Could not find component type {}", managedTypename);
						return;
					}
					s_EntityHasComponentFuncs[managedType] = [](Entity entity) { return entity.HasComponent<Component>(); };
				}(), ...);
		}

		template<typename... Component>
		static void RegisterComponent(ComponentGroup<Component...>)
		{
			RegisterComponent<Component...>();
		}

		void ScriptGlue::RegisterComponents()
		{
			s_EntityHasComponentFuncs.clear();
			RegisterComponent(AllComponents{});
		}

		static void NativeLog_Vector(glm::vec3* parameter, glm::vec3* outResult)
		{
			HE_CORE_WARN("Value: {0}", *parameter);
			*outResult = glm::normalize(*parameter);
		}

		static float NativeLog_VectorDot(glm::vec3* parameter)
		{
			HE_CORE_WARN("Value: {0}", *parameter);
			return glm::dot(*parameter, *parameter);
		}

		void ScriptGlue::RegisterFunctions()
		{
			HE_ADD_INTERNAL_CALL(NativeLog);
			HE_ADD_INTERNAL_CALL(NativeLog_Vector);
			HE_ADD_INTERNAL_CALL(NativeLog_VectorDot);

			HE_ADD_INTERNAL_CALL(GetScriptInstance);
			HE_ADD_INTERNAL_CALL(Entity_HasComponent);
			HE_ADD_INTERNAL_CALL(Entity_FindEntityByName);
			HE_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
			HE_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);

			HE_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulse);
			HE_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulseToCenter);
			HE_ADD_INTERNAL_CALL(Rigidbody2DComponent_GetLinearVelocity);
			HE_ADD_INTERNAL_CALL(Rigidbody2DComponent_GetType);
			HE_ADD_INTERNAL_CALL(Rigidbody2DComponent_SetType);

			HE_ADD_INTERNAL_CALL(TextComponent_GetText);
			HE_ADD_INTERNAL_CALL(TextComponent_SetText);
			HE_ADD_INTERNAL_CALL(TextComponent_GetColor);
			HE_ADD_INTERNAL_CALL(TextComponent_SetColor);
			HE_ADD_INTERNAL_CALL(TextComponent_GetKerning);
			HE_ADD_INTERNAL_CALL(TextComponent_SetKerning);
			HE_ADD_INTERNAL_CALL(TextComponent_GetLineSpacing);
			HE_ADD_INTERNAL_CALL(TextComponent_SetLineSpacing);

			HE_ADD_INTERNAL_CALL(Input_IsKeyDown);
		}

	}

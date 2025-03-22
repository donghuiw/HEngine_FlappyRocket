#pragma once

#include "HEngine/Core/KeyCodes.h"
#include "HEngine/Core/MouseCodes.h"

#include <glm/glm.hpp>

#include <utility>

namespace HEngine
{
	class  Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
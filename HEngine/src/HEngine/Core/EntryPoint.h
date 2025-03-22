#pragma once

#include "HEngine/Debug/Instrumentor.h"
#include "HEngine/Core/Base.h"
#include "HEngine/Core/Application.h"
#ifdef HE_PLATFORM_WINDOWS

extern HEngine::Application* HEngine::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	HEngine::Log::Init();

	HE_PROFILE_BEGIN_SESSION("Startup", "HEngineProfile-Startup.json");
	auto app = HEngine::CreateApplication({ argc, argv });
	HE_PROFILE_END_SESSION();

	HE_PROFILE_BEGIN_SESSION("Runtime", "HEngineProfile-Runtime.json");
	app->Run();
	HE_PROFILE_END_SESSION();

	HE_PROFILE_BEGIN_SESSION("Shutdown", "HEngineProfile-Shutdown.json");
	delete app;
	HE_PROFILE_END_SESSION();
}
#endif
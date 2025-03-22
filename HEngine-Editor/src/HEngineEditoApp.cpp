#include "HEngine.h"
#include "HEngine/Core/EntryPoint.h"

#include "EditorLayer.h"

namespace HEngine
{
	class HEngineEditor : public Application
	{
	public:
		HEngineEditor(const ApplicationSpecification& spec)
			: Application(spec)
		{
			PushLayer(new EditorLayer());
		}
	};
	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.Name = "HEngine-Editor";
		spec.CommandLineArgs = args;

		return new HEngineEditor(spec);
	}
}
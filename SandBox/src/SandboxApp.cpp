#include "HEngine.h"
#include "HEngine/Core/EntryPoint.h"

#include "Sandbox2D.h"
#include "ExampleLayer.h"

class SandBox : public HEngine::Application
{
public:
	SandBox(const HEngine::ApplicationSpecification& specification)
		: Application(specification)
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D);
	}
	~SandBox()
	{
	}
};
HEngine::Application* HEngine::CreateApplication(ApplicationCommandLineArgs args)
{
	ApplicationSpecification spec;
	spec.Name = "Sandbox";
	spec.WorkingDirectory = "../HEngine-Editor";
	spec.CommandLineArgs = args;
	
	return new SandBox(spec);
}

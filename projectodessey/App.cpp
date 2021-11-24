#include "App.h"
#include "EngineUtil.h"

App::App(const std::string& commandLine)
	:
	commandLine(commandLine),
	wnd(std::make_shared<Window>("Project Odessey")),
	scriptCommander(TokenizeQuoted(commandLine)),
	scene(std::make_unique<Scene>(wnd, "Scene\\scene.json")),
	gui(std::make_shared<GUISystem>(scene.get()))
{}

void App::HandleInput(float dt)
{
	scene->ProcessInput(dt);
}

void App::DoFrame(float dt)
{
	wnd->Gfx().BeginFrame();

	gui->Show();
	scene->Render(dt);

	wnd->Gfx().EndFrame();
}

int App::Go()
{
	while( true )
	{
		// Обработка всех сообщений
		if(const auto ecode = Window::ProcessMessages())
		{
			// Если optional имеет значение, выходим
			return *ecode;
		}
		
		// Логика игры
		const auto dt = timer.Mark() * speed_factor;
		
		HandleInput(dt);
		DoFrame(dt);
	}
}
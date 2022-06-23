#include "Scene.h"

#include "EngineUtil.h"
#include "Channels.h"
#include "Camera.h"

#include <sstream>

Scene::Scene(std::shared_ptr<Window> wnd, std::string data)
	:
	wnd(wnd),
	objects(data, wnd->Gfx(), rg),
	robot("SevROV", "Scene\\Models\\Robot\\robot.obj", wnd->Gfx(), wnd,
		DirectX::XMFLOAT3({ 0.0f, 0.0f, 0.0f }),
		DirectX::XMFLOAT3({ 0.0f, 0.0f, 0.0f }), 0.01f),
	sim("model\\position.txt", "model\\orientation.txt", "model\\time.txt", robot, wnd->Gfx(), rg)
{	
	objects.cameras.AddCamera(robot.GetCamera());
	robot.AttachGfx(rg);
	sim.LinkTechniques();
	objects.LinkTechniques(rg);
}

void Scene::ProcessInput(float dt)
{
	while (const auto e = wnd->kbd.ReadKey())
	{
		if (!e->IsPress())
		{
			continue;
		}

		switch (e->GetCode())
		{
		case VK_ESCAPE:
			if (wnd->CursorEnabled())
			{
				wnd->DisableCursor();
				wnd->mouse.EnableRaw();
			}
			else
			{
				wnd->EnableCursor();
				wnd->mouse.DisableRaw();
			}			
			break;
		default:
			break;
		}
	}

	if (!wnd->CursorEnabled())
	{
		if (wnd->kbd.KeyIsPressed('W'))
		{
			objects.cameras->Translate({ 0.0f,0.0f,dt });
		}
		if (wnd->kbd.KeyIsPressed('A'))
		{
			objects.cameras->Translate({ -dt,0.0f,0.0f });
		}
		if (wnd->kbd.KeyIsPressed('S'))
		{
			objects.cameras->Translate({ 0.0f,0.0f,-dt });
		}
		if (wnd->kbd.KeyIsPressed('D'))
		{
			objects.cameras->Translate({ dt,0.0f,0.0f });
		}
		if (wnd->kbd.KeyIsPressed('R'))
		{
			objects.cameras->Translate({ 0.0f,dt,0.0f });
		}
		if (wnd->kbd.KeyIsPressed('F'))
		{
			objects.cameras->Translate({ 0.0f,-dt,0.0f });
		}
	}

	while (const auto delta = wnd->mouse.ReadRawDelta())
	{
		if (!wnd->CursorEnabled())
		{
			objects.cameras->Rotate((float)delta->x, (float)delta->y);
		}
	}

	if (!objects.plates.CheckCollision(robot.GetHitBox()))
	{
		sim.Simulate(dt);
	}
	else
	{
		sim.Stop(SimFlag::Collision);
	}
}

void Scene::Render(float dt)
{
	objects.pLight.Bind(wnd->Gfx(), objects.cameras->GetMatrix());
	rg.BindMainCamera(objects.cameras.GetActiveCamera());

	objects.Submit(Chan::main);
	objects.models.Submit(Chan::shadow);

	sim.Draw(Chan::main);

	robot.Render(Chan::main);
	robot.Render(Chan::shadow);

	rg.Execute(wnd->Gfx());

	rg.Reset();
}
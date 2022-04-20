#include "Scene.h"

#include "EngineUtil.h"
#include "Channels.h"
#include "Camera.h"

#include <sstream>

#define GRID_WIDTH  20
#define GRID_HEIGHT 20
#define CELL_WIDTH  20.0f
#define CELL_HEIGHT 20.0f

Scene::Scene(std::shared_ptr<Window> wnd, std::string data)
	:
	wnd(wnd),
	objects(data, wnd->Gfx(), rg),
	robot("SevROV", "Scene\\Models\\Robot\\robot.obj", wnd->Gfx(), wnd,
		DirectX::XMFLOAT3({ 0.0f, 0.0f, 0.0f }),
		DirectX::XMFLOAT3({ 0.0f, 0.0f, 0.0f }), 0.01f),
	sim("Data\\position.txt", "Data\\orientation.txt", "Data\\time.txt", robot, wnd->Gfx())
{	
	for (size_t i = 0; i < GRID_HEIGHT; i++)
	{
		for (size_t j = 0; j < GRID_WIDTH; j++)
		{
			grid.push_back(std::make_unique<Plate>(wnd->Gfx(), 
				DirectX::XMFLOAT2{ GRID_WIDTH, GRID_HEIGHT },
				DirectX::XMFLOAT3{ i * CELL_HEIGHT - GRID_HEIGHT * CELL_HEIGHT / 2.0f, 0.0f, j * CELL_WIDTH - GRID_WIDTH * CELL_WIDTH / 2.0f },
				DirectX::XMFLOAT3{ 3.14 / 2, 0.0f, 0.0f }));
		}
	}

	for (auto& c : grid)
	{
		c->LinkTechniques(rg);
	}
	
	objects.cameras.AddCamera(robot.GetCamera());
	robot.AttachGfx(rg);
	sim.LinkTechniques(rg);
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
		case 0x45:
			sim.Start();
			break;
		case 0x46:
			sim.Stop();
			break;
		case VK_RETURN:
			savingDepth = true;
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

	sim.Simulate(dt);
}

void Scene::Render(float dt)
{
	objects.pLight.Bind(wnd->Gfx(), objects.cameras->GetMatrix());
	rg.BindMainCamera(objects.cameras.GetActiveCamera());

	objects.Submit(Chan::main);
	objects.models.Submit(Chan::shadow);

	for (auto& c : grid)
	{
		c->Submit(Chan::main);
	}

	sim.Draw(Chan::main);

	robot.Render(Chan::main);
	robot.Render(Chan::shadow);

	rg.Execute(wnd->Gfx());

	if (savingDepth)
	{
		rg.DumpShadowMap(wnd->Gfx(), "shadow.png");
		savingDepth = false;
	}

	rg.Reset();
}

std::pair<std::string, bool> Scene::IsOnTheSceneTrigger()
{
	return objects.triggers.Check(objects.cameras.GetActiveCamera().GetPosition());
}

void Scene::ResetPos()
{
	objects.cameras.GetActiveCamera().SetPosition(dx::XMFLOAT3{ -13.5f,6.0f,3.5f });
}
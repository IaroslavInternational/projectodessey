#include "Robot.h"

Robot::Robot(std::string name, const std::string& path, Graphics& gfx, std::shared_ptr<Window> wnd,
	   DirectX::XMFLOAT3 position,
	   DirectX::XMFLOAT3 orientation,
	   float scale)
	:
	wnd(wnd)
{
	model = std::make_unique<Model>(name, path, gfx, position, orientation, scale);
	camera = std::make_shared<Camera>(name + std::string(" cam"), gfx, position, orientation);
	camera->SetTravelSpeed(1.0f);
}

void Robot::AttachGfx(Rgph::RenderGraph& rg)
{
	model->LinkTechniques(rg);
}

std::shared_ptr<Camera> Robot::GetCamera()
{
	return camera;
}

void Robot::HandleInput(float dt)
{
	dt *= speed;

	if (wnd->kbd.KeyIsPressed('W'))
	{
		model->Translate({ 0.0f,0.0f,dt });
		camera->Translate({ 0.0f,0.0f,dt });
	}
	if (wnd->kbd.KeyIsPressed('A'))
	{
		model->Translate({ -dt,0.0f,0.0f });
		camera->Translate({ -dt,0.0f,0.0f });
		
		if (model->GetOrientation().z <= 45.0f * 0.017f)
		{
			model->Rotate({0.0f, 0.0f, dt});
		}
	}
	if (wnd->kbd.KeyIsPressed('S'))
	{
		model->Translate({ 0.0f,0.0f,-dt });
		camera->Translate({ 0.0f,0.0f,-dt });
	}
	if (wnd->kbd.KeyIsPressed('D'))
	{
		model->Translate({ dt,0.0f,0.0f });
		camera->Translate({ dt,0.0f,0.0f });

		if (model->GetOrientation().z >= -45.0f * 0.017f)
		{
			model->Rotate({ 0.0f, 0.0f, -dt });
		}
	}
	if (wnd->kbd.KeyIsPressed('R'))
	{
		model->Translate({ 0.0f,dt,0.0f });
		camera->Translate({ 0.0f,dt,0.0f });
	}
	if (wnd->kbd.KeyIsPressed('F'))
	{
		model->Translate({ 0.0f,-dt,0.0f });
		camera->Translate({ 0.0f,-dt,0.0f });
	}
}

void Robot::Render(size_t channel)
{
	model->Submit(channel);
}

DirectX::XMFLOAT3 Robot::GetPosition()
{
	return model->GetPosition();
}

DirectX::XMFLOAT3 Robot::GetOrientation()
{
	return model->GetOrientation();
}

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
	if (!IsManualControl)
	{
		return;
	}

	DirectX::XMFLOAT3 translation = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f };

	dt *= speed;

	if (wnd->kbd.KeyIsPressed('W'))
	{
		translation = { 0.0f,0.0f,dt };
	}
	else if (wnd->kbd.KeyIsPressed('A'))
	{
		translation = { -dt,0.0f,0.0f };
		
	}
	else if (wnd->kbd.KeyIsPressed('S'))
	{
		translation = { 0.0f,0.0f,-dt };		
	}
	else if (wnd->kbd.KeyIsPressed('D'))
	{
		translation = { dt,0.0f,0.0f };
	}
	else if (wnd->kbd.KeyIsPressed('R'))
	{
		translation = { 0.0f,dt,0.0f };
	}
	else if (wnd->kbd.KeyIsPressed('F'))
	{
		translation = { 0.0f,-dt,0.0f };
	}

	UpdateModel(translation, rotation);
}

void Robot::UpdateModel(DirectX::XMFLOAT3 translation, DirectX::XMFLOAT3 rotation)
{
	if (translation.x != 0.0f &&
		translation.y != 0.0f &&
		translation.z != 0.0f)
	{

	}
	else 
	{
		Translate(translation);
	}
	
	if (rotation.x != 0.0f &&
		rotation.y != 0.0f &&
		rotation.z != 0.0f)
	{

	}
	else
	{
		Rotate(rotation);
	}
}

void Robot::Render(size_t channel)
{
	model->Submit(channel);
}

void Robot::Translate(DirectX::XMFLOAT3 translation)
{
	model->Translate(translation);
	camera->Translate(translation);
}

void Robot::Rotate(DirectX::XMFLOAT3 rotation)
{
	model->Rotate(rotation);
}

DirectX::XMFLOAT3 Robot::GetPosition()
{
	return model->GetPosition();
}

DirectX::XMFLOAT3 Robot::GetOrientation()
{
	return model->GetOrientation();
}

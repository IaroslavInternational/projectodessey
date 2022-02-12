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

	dt *= linear_speed;
	
	if (wnd->kbd.KeyIsPressed('W'))
	{
		translation.z += dt;
	}
	else if (wnd->kbd.KeyIsPressed('S'))
	{
		translation.z += -dt;
	}

	if (wnd->kbd.KeyIsPressed('A'))
	{
		translation.x += -dt;
		rotation.z += dt / 10.0f;
	}
	else if (wnd->kbd.KeyIsPressed('D'))
	{
		translation.x += dt;
		rotation.z += -dt / 10.0f;
	}

	if (wnd->kbd.KeyIsPressed('R'))
	{
		translation.y += dt;
	}
	else if (wnd->kbd.KeyIsPressed('F'))
	{
		translation.y += -dt;
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
	if (!(GetOrientation().x >= -0.79f && GetOrientation().x <= 0.79f))
	{
		if (GetOrientation().x < 0.0f)
		{
			model->SetOrientation({ -0.79f, GetOrientation().y, GetOrientation().z });
		}
		else
		{
			model->SetOrientation({ 0.79f, GetOrientation().y, GetOrientation().z });
		}

		rotation.x = 0.0f;
	}

	if (!(GetOrientation().y >= -0.79f && GetOrientation().y <= 0.79f))
	{
		if (GetOrientation().y < 0.0f)
		{
			model->SetOrientation({ GetOrientation().x, -0.79f, GetOrientation().z });
		}
		else
		{
			model->SetOrientation({ GetOrientation().x, 0.79f, GetOrientation().z });
		}

		rotation.y = 0.0f;
	}

	if (!(GetOrientation().z > -0.79f && GetOrientation().z < 0.79f))
	{
		if (GetOrientation().z < 0.0f)
		{
			model->SetOrientation({ GetOrientation().x, GetOrientation().y, -0.79f });
		}
		else
		{
			model->SetOrientation({ GetOrientation().x, GetOrientation().y, 0.79f });
		}

		rotation.z = 0.0f;
	}

	model->Rotate(rotation);
}

void Robot::SetManualControl(bool state)
{
	IsManualControl = state;
}

DirectX::XMFLOAT3 Robot::GetPosition()
{
	return model->GetPosition();
}

DirectX::XMFLOAT3 Robot::GetOrientation()
{
	return model->GetOrientation();
}

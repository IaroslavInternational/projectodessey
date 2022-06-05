#include "Robot.h"

Robot::Robot(std::string name, const std::string& path, Graphics& gfx, std::shared_ptr<Window> wnd,
	   DirectX::XMFLOAT3 position,
	   DirectX::XMFLOAT3 orientation,
	   float scale)
	:
	wnd(wnd),
	hb(position)
{
	model = std::make_unique<Model>(name, path, gfx, position, orientation, scale);
	camera = std::make_shared<Camera>(name + std::string(" cam"), gfx, position, orientation);
	camera->SetTravelSpeed(1.0f);
}

void Robot::AttachGfx(Rgph::RenderGraph& rg)
{
	model->LinkTechniques(rg);
}

void Robot::Render(size_t channel)
{
	model->Submit(channel);
}

std::shared_ptr<Camera> Robot::GetCamera()
{
	return camera;
}

void Robot::SetPosition(DirectX::XMFLOAT3 position)
{
	model->SetPosition(position);
	camera->SetPosition(position);
	hb.Translate({ position.x, position.y, position.z});
}

void Robot::SetOrientation(DirectX::XMFLOAT3 orientation)
{
	model->SetOrientation(orientation);
	camera->SetOrientation(orientation);
}

DirectX::XMFLOAT3 Robot::GetPosition()
{
	return model->GetPosition();
}

DirectX::XMFLOAT3 Robot::GetOrientation()
{
	return model->GetOrientation();
}

bool Robot::IsCollide(Plate& plate)
{
	return hb.IsCollide(plate);
}

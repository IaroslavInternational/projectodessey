#include "Robot.h"

Robot::Robot(std::string name, const std::string& path, Graphics& gfx,
	   DirectX::XMFLOAT3 position,
	   DirectX::XMFLOAT3 orientation,
	   float scale)
{
	model = std::make_unique<Model>(name, path, gfx, position, orientation, scale);
	camera = std::make_shared<Camera>(name + std::string(" cam"), gfx, position, orientation);
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

#pragma once

#include "Model.h"
#include "Camera.h"
#include <memory>

class Robot
{
public:
	Robot(std::string		name, const std::string& path, Graphics& gfx,
		  DirectX::XMFLOAT3 position	= { 0.0f, 0.0f, 0.0f },
		  DirectX::XMFLOAT3 orientation = { 0.0f, 0.0f, 0.0f },
		  float				scale = 1.0f);
public:
	void AttachGfx(Rgph::RenderGraph& rg);
	void Render(size_t channel);
	std::shared_ptr<Camera> GetCamera();
private:
	std::unique_ptr<Model> model;
	std::shared_ptr<Camera> camera;
};


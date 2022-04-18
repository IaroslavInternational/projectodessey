#pragma once

#include "Model.h"
#include "Camera.h"
#include <memory>
#include "Window.h"

class Robot
{
public:
	friend class GUISystem;
public:
	Robot(std::string		name, const std::string& path, Graphics& gfx, std::shared_ptr<Window> wnd,
		  DirectX::XMFLOAT3 position	= { 0.0f, 0.0f, 0.0f },
		  DirectX::XMFLOAT3 orientation = { 0.0f, 0.0f, 0.0f },
		  float				scale = 1.0f);
public:
	void					AttachGfx(Rgph::RenderGraph& rg);
	void					Render(size_t channel);
	std::shared_ptr<Camera> GetCamera();
public:
	void			  Translate(DirectX::XMFLOAT3 translation);
	void			  Rotate(DirectX::XMFLOAT3 rotation);
	void			  SetPosition(DirectX::XMFLOAT3 position);
	void			  SetOrientation(DirectX::XMFLOAT3 orientation);
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetOrientation();
private:
	std::unique_ptr<Model>  model;
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Window> wnd;
};


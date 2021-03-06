#pragma once

#include "Model.h"
#include "Camera.h"
#include <memory>
#include "Window.h"
#include "HitBox.h"

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
	void			  SetPosition(DirectX::XMFLOAT3 position);
	void			  SetOrientation(DirectX::XMFLOAT3 orientation);
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetOrientation();
	HitBox&			  GetHitBox();
	bool			  IsCollide(Plate& plate);
private:
	std::unique_ptr<Model>  model;
	std::unique_ptr<Model>  axis;
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Window> wnd;
	HitBox hb;
	DirectX::XMFLOAT3 cam_offset = { 0.0f, -8.5f, 15.0f };
};


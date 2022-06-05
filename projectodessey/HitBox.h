#pragma once

#include "Plate.h"
#include <DirectXCollision.h>

class HitBox
{
public:
	friend class GUISystem;
public:
	HitBox(DirectX::XMFLOAT3 center, float radius = 2.0f);
public:
	bool IsCollide(const DirectX::BoundingBox& box);
	bool IsCollide(Plate& box);
public:
	void Draw();
	void Translate(DirectX::XMFLOAT3 position);
private:
	DirectX::BoundingSphere sphere;
};

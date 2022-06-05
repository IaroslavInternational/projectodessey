#include "HitBox.h"

HitBox::HitBox(DirectX::XMFLOAT3 center, float radius)
{
	sphere.Center = center;
	sphere.Radius = radius;
}

bool HitBox::IsCollide(const DirectX::BoundingBox& box)
{
	return sphere.Intersects(box);
}

bool HitBox::IsCollide(Plate& box)
{
	return IsCollide(box.CreateBBox());
}

void HitBox::Translate(DirectX::XMFLOAT3 position)
{
	sphere.Transform(sphere, 1.0f, { 0.0f, 0.0f, 0.0f }, { position.x, position.y, position.z });
}

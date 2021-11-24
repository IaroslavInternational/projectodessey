#include "VisibleObject.h"

VisibleObject::VisibleObject(std::string name, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 orientation, bool visibility)
	:
	Object(name, position, orientation),
	visibility(visibility)
{
}

void VisibleObject::SetVisibility(bool visibility)
{
	this->visibility = visibility;
}

bool VisibleObject::IsVisible() const
{
	return visibility;
}

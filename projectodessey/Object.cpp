#include "Object.h"

Object::Object(std::string name, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 orientation)
	:
	name(name),
	position(position),
	orientation(orientation)
{
}

std::string Object::GetName() const
{
	return name;
}

DirectX::XMFLOAT3 Object::GetPosition() const
{
	return position;
}

DirectX::XMFLOAT3 Object::GetOrientation() const
{
	return orientation;
}

void Object::SetName(std::string name)
{
	this->name = name;
}

void Object::SetPosition(DirectX::XMFLOAT3 position)
{
	this->position = position;
}

void Object::SetOrientation(DirectX::XMFLOAT3 orientation)
{
	this->orientation = orientation;
}

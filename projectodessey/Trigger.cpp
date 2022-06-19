#include "Trigger.h"
#include "EngineXM.h"
#include "Channels.h"

#include <sstream>

Trigger::Trigger(std::string	   name,
				 Graphics&		   gfx,
				 DirectX::XMFLOAT2 size,
				 DirectX::XMFLOAT3 position,
				 DirectX::XMFLOAT3 orientation,
				 bool			   visibility)
	:
	VisibleObject(name, position, orientation, visibility),
	size(size),
	platform(gfx, size, position, orientation, {0.0f, 0.0f, 0.0f, 1.0f})
{
	full_position.UpdatePoints(position, size);
}

void Trigger::LinkTechniques(Rgph::RenderGraph& rg)
{
	platform.LinkTechniques(rg);
}

void Trigger::Submit(size_t channels)
{
	platform.Submit(channels);
}

void Trigger::SetPosition(DirectX::XMFLOAT3 position)
{
	platform.SetPosition(position);
}

void Trigger::SetOrientation(DirectX::XMFLOAT3 orientation )
{
	platform.SetRotation(orientation);
}

void Trigger::SetDefault()
{
	full_position.UpdatePoints(position, platform.GetSize());

	platform.SetPosition(position);
	platform.SetRotation(orientation);
}

bool Trigger::Check(DirectX::XMFLOAT3 ObjPos)
{
	objpos = ObjPos;
	return (ObjPos.x >=	   this->full_position.PosTopLeft.x
			&& ObjPos.x <= this->full_position.PosTopLeft.x + deep) &&
			ObjPos.y <=	   this->full_position.PosTopLeft.y &&
			ObjPos.z <=	   this->full_position.PosTopLeft.z &&

			(ObjPos.x >=   this->full_position.PosTopRight.x
			&& ObjPos.x <= this->full_position.PosTopRight.x + deep) &&
			ObjPos.y <=    this->full_position.PosTopRight.y &&
			ObjPos.z >=    this->full_position.PosTopRight.z &&

			(ObjPos.x >=   this->full_position.PosBottomLeft.x
			&& ObjPos.x <= this->full_position.PosBottomLeft.x + deep) &&
			ObjPos.y >=    this->full_position.PosBottomLeft.y &&
			ObjPos.z <=    this->full_position.PosBottomLeft.z &&

			(ObjPos.x >=   this->full_position.PosBottomRight.x
			&& ObjPos.x <= this->full_position.PosBottomRight.x + deep) &&
			ObjPos.y >=	   this->full_position.PosBottomRight.y &&
			ObjPos.z >=	   this->full_position.PosBottomRight.z;
}

void Trigger::SetDeep(float TriggerDeep)
{
	deep = TriggerDeep;
}

const DirectX::XMFLOAT3* Trigger::GetPosition() const
{
	DirectX::XMFLOAT3 arr[] = 
	{ 
		full_position.PosTopLeft,	 full_position.PosTopRight,
		full_position.PosBottomLeft, full_position.PosBottomRight 
	};

	return arr;
}

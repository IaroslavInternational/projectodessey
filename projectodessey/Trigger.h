#pragma once

#include "TriggerStruct.h"
#include "BlurOutlineRenderGraph.h"
#include "VisibleObject.h"

#include <DirectXMath.h>

#include "Plate.h"

// Ѕазовый класс дл€ триггеров
class Trigger : public VisibleObject
{
public:
	friend class GUISystem;
public:
	Trigger(std::string		  name, 
			Graphics&		  gfx,
			DirectX::XMFLOAT2 size,
			DirectX::XMFLOAT3 position =	{ 0.0f, 0.0f, 0.0f },
			DirectX::XMFLOAT3 orientation = { 0.0f, 1.57f, 0.0f },
			bool			  visibility =  true);

	void LinkTechniques(Rgph::RenderGraph& rg);

	void Submit(size_t channels);
public:	
	void SetDefault();// ”становка начального положение
	void SetPosition(DirectX::XMFLOAT3 position);// ѕерегруженный метод дл€ установки позиции триггера, вклю€ча€ платформу
	void SetOrientation(DirectX::XMFLOAT3 orientation);// ѕерегруженный метод дл€ установки ориентации триггера, вклю€ча€ платформу	
	bool Check(DirectX::XMFLOAT3 ObjPos);// ѕроверка на касание триггера через координаты
	void SetDeep(float TriggerDeep);// ”становка ширины триггера
	const DirectX::XMFLOAT3* GetPosition() const;// ѕолучение позиции триггера
private:
	float deep = 2.5f;	// √лубина триггера по x
private:
	struct
	{
		void UpdatePoints(DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 size) noexcept
		{
			PosTopLeft =	 { position.x, position.y,			position.z };
			PosTopRight =	 { position.x, position.y,			position.z - size.x };
			PosBottomLeft =  { position.x, position.y - size.y, position.z };
			PosBottomRight = { position.x, position.y - size.y, position.z - size.x };
		}

		DirectX::XMFLOAT3 PosTopLeft;
		DirectX::XMFLOAT3 PosTopRight;
		DirectX::XMFLOAT3 PosBottomLeft;
		DirectX::XMFLOAT3 PosBottomRight;
	} full_position;

	DirectX::XMFLOAT3 objpos = { 0.0f, 0.0f, 0.0f };
	// ¬изуальный контур триггера
	Plate platform;

	DirectX::XMFLOAT2 size;
};
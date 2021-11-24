#pragma once

#include <DirectXMath.h>

namespace dx = DirectX;

// Структура координат триггера
struct TriggerStruct
{
	TriggerStruct() = default;

	TriggerStruct(dx::XMFLOAT3 & ptl, dx::XMFLOAT3 & ptr,
		dx::XMFLOAT3 & pbl, dx::XMFLOAT3 & pbr,
		float& r, float& p, float& y)
	{
		PosTopLeft = ptl;
		PosTopRight = ptr;
		PosBottomLeft = pbl;
		PosBottomRight = pbr;

		Roll = r;
		Pitch = p;
		Yaw = y;
	}
	
	dx::XMFLOAT3 PosTopLeft;	 // Позиция левого верхнего угла
	dx::XMFLOAT3 PosTopRight;	 // Позиция правого верхнего угла
	dx::XMFLOAT3 PosBottomLeft;  // Позиция левого нижнего угла
	dx::XMFLOAT3 PosBottomRight; // Позиция правого нижнего угла

	float Roll;
	float Pitch;
	float Yaw;

	// Оператор присваивания
	TriggerStruct& operator=(const TriggerStruct& rhs)
	{
		PosTopLeft = rhs.PosTopLeft;
		PosTopRight = rhs.PosTopRight;
		PosBottomLeft = rhs.PosBottomLeft;
		PosBottomRight = rhs.PosBottomRight;

		Roll = rhs.Roll;
		Pitch = rhs.Pitch;
		Yaw = rhs.Yaw;

		return *this;
	}

	// Оператор сравнения
	bool operator==(const TriggerStruct& rhs) const
	{
		return PosTopLeft.x == rhs.PosTopLeft.x &&
			PosTopLeft.y == rhs.PosTopLeft.y &&
			PosTopLeft.z == rhs.PosTopLeft.z &&
			PosTopRight.x == rhs.PosTopRight.x &&
			PosTopRight.y == rhs.PosTopRight.y &&
			PosTopRight.z == rhs.PosTopRight.z &&
			PosBottomLeft.x == rhs.PosBottomLeft.x &&
			PosBottomLeft.y == rhs.PosBottomLeft.y &&
			PosBottomLeft.z == rhs.PosBottomLeft.z &&
			PosBottomRight.x == rhs.PosBottomRight.x &&
			PosBottomRight.y == rhs.PosBottomRight.y &&
			PosBottomRight.z == rhs.PosBottomRight.z &&

			Roll == rhs.Roll &&
			Pitch == rhs.Pitch &&
			Yaw == rhs.Yaw;
	}

	// Оператор не равно
	bool operator!=(const TriggerStruct& rhs) const
	{
		return !(*this == rhs);
	}
};

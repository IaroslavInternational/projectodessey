#pragma once

#include "Drawable.h"
#include "IndexedTriangleList.h"

// Представляет собой графический объект на сцене в виде не осязаемой прозрачной платформы,
// имеющей контур. Используется в классе Trigger для отображения области распространения действия триггера
class Plate : public Drawable
{
public:
	friend class GUISystem;
public:
	// Конструктор платоформы требует следующие поля:
	// gfx - адрес графического ядра
	// position - позиция платформы
	// orientation - ориентация платформы
	Plate(Graphics&		    gfx, 
		  DirectX::XMFLOAT2 size, 
		  DirectX::XMFLOAT3 position =	  { 0.0f, 0.0f, 0.0f },
		  DirectX::XMFLOAT3 orientation = { 0.0f, 0.0f, 0.0f},
		  DirectX::XMFLOAT4 color =		  { 0.0f, 192.0f, 204.0f, 0.0f });
public:
	// Установить размер
	// size.x - ширина,
	// size.y - высота
	void			  SetSize(DirectX::XMFLOAT2 size);

	// Установить позцию
	void			  SetPosition(DirectX::XMFLOAT3 position) noexcept;

	// Установить ориентацию
	void			  SetRotation(DirectX::XMFLOAT3 orientation) noexcept;

	void			  UpdateSize(DirectX::XMFLOAT2 size);

	// Получить размер
	// size.x - ширина,
	// size.y - высота
	DirectX::XMFLOAT2 GetSize() const noexcept;

	// Получить указатель на размер
	// size.x - ширина,
	// size.y - высота
	DirectX::XMFLOAT2* GetSizePtr() noexcept;

	// Получить матрицу трансформации платформы
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	void			  CreatePlate();
private:
	Graphics& gfx;

	// Цвет контура
	struct PSMaterialConstant
	{
		DirectX::XMFLOAT4 color;
	} pmc;

	// Размер платформы
	DirectX::XMFLOAT2 size;

	// Позциия платформы
	DirectX::XMFLOAT3 position;

	// Ориентация платформы
	DirectX::XMFLOAT3 orientation;

	IndexedTriangleList model;
};
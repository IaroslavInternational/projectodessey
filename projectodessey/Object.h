#pragma once

#include "Graphics.h"

// Базовый класс для всех объектов на сцене. От этого класса наследуются остальные производные объекты.
class Object
{
public:
	// Конструктор объекта требует следующие поля:
	// name - имя объекта
	// position - позиция объекта
	// orientation - ориентация объекта
	Object(std::string name, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 orientation);
public:
	// Получить имя объекта
	std::string		  GetName() const;

	// Получить позицию объекта
	DirectX::XMFLOAT3 GetPosition() const;
	
	// Получить ориентацию объекта
	DirectX::XMFLOAT3 GetOrientation() const;
	
	// Установить имя объекта
	void			  SetName(std::string name);

	// Установить позицию объекта
	void			  SetPosition(DirectX::XMFLOAT3 position);
	
	// Установить ориентацию объекта
	void			  SetOrientation(DirectX::XMFLOAT3 orientation);
protected:
	// Имя объекта
	std::string		  name;
	
	// Позиция объекта
	DirectX::XMFLOAT3 position;

	// Ориентация объекта
	DirectX::XMFLOAT3 orientation;
};

#pragma once

#include "Object.h"

namespace Rgph
{
	class RenderGraph;
}

// Базовый класс для видимых объектов на сцене. В отличие от Object здесь появляются методы для отрисовки.
class VisibleObject : public Object
{
public:
	// Конструктор видимого объекта требует следующие поля:
	// name - имя объекта
	// position - позиция объекта
	// orientation - ориентация объекта
	// visibility - видимость объекта
	VisibleObject(std::string name, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 orientation, bool visibility);

	// Виртуальный деструктор
	virtual ~VisibleObject() {};
public:
	// Виртуальный метод для присоединения объекта к Render Graph
	virtual void LinkTechniques(Rgph::RenderGraph& rg) = 0;

	// Виртуальный метод для присоединения объекта к каналу отрисовки
	virtual void Submit(size_t channels) = 0;

	// Установить видимость объекта
	void		 SetVisibility(bool visibility);

	// Проверка видимости объекта
	bool		 IsVisible() const;
protected:
	// Видимость объекта
	bool visibility;
};
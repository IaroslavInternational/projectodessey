#pragma once

#include "Trigger.h"

#include <memory>
#include <vector>

// Триггеры для перехода на следующую сцену
class TriggerContainer
{
public:
	friend class GUISystem;
public:
	TriggerContainer(std::string path, Graphics& gfx, Rgph::RenderGraph& rg);
public:
	void LinkTechniques();
	void Submit(size_t channels);
public:
	std::pair<std::string, bool> Check(dx::XMFLOAT3 pos); // Имя цели, статус | Проверка на пересечение триггера
private:
	void AddTrigger(std::string name, std::string ptr); //
	void DeleteTrigger(std::string name);				// Удалить модель
private:
	std::string path;								// Путь к файлу с данными о моделях
	Graphics& gfx;									// Адрес графичсекого ядра
	Rgph::RenderGraph& rg;							// Адрес рендер-графа
	std::vector<std::unique_ptr<Trigger>> triggers;	// Триггеры
};
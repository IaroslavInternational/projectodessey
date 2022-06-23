#pragma once

#include "Plate.h"
#include "HitBox.h"

class PlateContainer
{
public:
	friend class GUISystem;
public:
	PlateContainer(std::string path, Graphics& gfx, Rgph::RenderGraph& rg);
public:
	void LinkTechniques();												// Добавить к рендеру
	void Submit(size_t channels);										// Добавить к каналу отрисовки
	bool CheckCollision(HitBox& hb);
private:
	std::string							path;							// Путь к файлу с данными
	Graphics&							gfx;							// Адрес графичсекого ядра
	Rgph::RenderGraph&					rg;								// Адрес рендер-графа
	std::vector<std::unique_ptr<Plate>> plates;							// Указатели на платформы
private:
	std::string selected = "";											// Выбранные элемент в списке моделей
};


#pragma once

#include "Model.h"

class ModelContainer
{
public:
	friend class GUISystem;
public:
	ModelContainer(std::string path, Graphics& gfx, Rgph::RenderGraph& rg);
public:
	void LinkTechniques();												// Добавить к рендеру
	void Submit(size_t channels);										// Добавить к каналу отрисовки
public:
	std::string GetModelNameByIndex(size_t index);						// Имя модели по индексу
public:
//	void OpenDialog();													// Открыть диалоговое окно для добавления модели на сцену
	size_t ModelsAmount();												// Кол-во моделей
private:
	void LoadModel(std::string name, std::string path);					// Добавить модель
	void DeleteModel(std::string name);									// Удалить модель
	void Init();														// Установить положение и ориентацию модели
private:
	std::string path;													// Путь к файлу с данными о моделях
	Graphics& gfx;														// Адрес графичсекого ядра
	Rgph::RenderGraph& rg;												// Адрес рендер-графа
	std::vector<std::unique_ptr<Model>> models;							// Указатели на модели
private:
	std::string selected = "";											// Выбранные элемент в списке моделей
	bool IsSave = false;												// Если нажата кнопка сохранить
	bool IsDelete = false;												// Если нажата кнопка удалить
};
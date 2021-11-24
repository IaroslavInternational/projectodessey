#pragma once

#include <string>
#include <vector>
#include <memory>

class Camera;
class Graphics;

namespace Rgph
{
	class RenderGraph;
}

class CameraContainer
{
public:
	friend class GUISystem;
public:
	CameraContainer(std::string path, Graphics& gfx, Rgph::RenderGraph& rg);
public:
	Camera* operator->();											// Указатель на камеру
	void LinkTechniques();
	void Submit(size_t channels) const;								// Добавить к каналу отрисовки
	void Bind();													// Установка камеры в графическом ядре
public:
	Camera& GetActiveCamera();
public:
	size_t CamerasAmount();											// Кол-во камер
	std::string GetCameraNameByIndex(size_t index);					// Имя камеры по индексу в контейнере
public:
	void ShowLeftPanel();											// Показать левую панель для камер
	void ShowRightPanel();											// Показать правую панель для камер
	void AddCamera(std::shared_ptr<Camera> pCam);					// Добавить камеру
	void AddRuntimeCamera(std::shared_ptr<Camera> pCam);			// Добавить камеру и записать в файл
private:
	void DeleteCamera(std::string name);							// Удалить камеру
	Camera& GetControlledCamera();									// Адрес управляемой камеры
private:
	std::string path = "";											// Путь к файлу с данными о камерах
	Graphics& gfx;													// Адрес графичсекого ядра
	Rgph::RenderGraph& rg;											// Адрес рендер-графа
	std::vector<std::shared_ptr<Camera>> cameras;					// Камеры
private:
	size_t active     = 0;											// Индекс активной камеры
	size_t controlled = 0;											// Индекс управляемой камеры
};
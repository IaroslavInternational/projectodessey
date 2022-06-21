#pragma once

#include "VisibleObject.h"
#include "Projection.h"
#include "CameraIndicator.h"

#include <DirectXMath.h>

class Graphics;
namespace Rgph
{
	class RenderGraph;
}

// Класс для описания функционала камер. 
class Camera : public VisibleObject
{
public:
	friend class GUISystem;
public:
	// Конструктор модели требует следующие поля:
	// name - имя камеры
	// gfx - адрес графического ядра
	// position - позиция камеры
	// orientation - ориентация камеры
	// prd - данные о проекции камеры
	// visibility - видимость камеры
	// tethered - режим ориентации камеры
	Camera(std::string		 name,
		   Graphics&		 gfx,
		   DirectX::XMFLOAT3 position =	   { 0.0f, 0.0f, 0.0f },
		   DirectX::XMFLOAT3 orientation = { 0.0f, 0.0f, 0.0f },
		   ProjectionData	 prd =		   { 1.0f, 9.0f / 16.0f, 0.5f, 400.0f },
		   bool				 visibility =  true,   
		   bool				 tethered =	   false) noexcept;
public:
	// Метод для присоединения модели к Render Graph
	void			  LinkTechniques(Rgph::RenderGraph& rg);

	// Метод для присоединения модели к каналу отрисовки
	void			  Submit(size_t channel);		

	// Метод для добавления камеры к цели рендера
	void			  BindToGraphics(Graphics& gfx) const;				

	// Сброс настроек камеры к начальным,
	// gfx нужен для отображения индикатора и проекции
	void			  Reset(Graphics& gfx) noexcept;

	// Вращение камеры
	void			  Rotate(float dx, float dy) noexcept;

	// Перемещение камеры
	void			  Translate(DirectX::XMFLOAT3 translation) noexcept;

	// Получение матрицы камеры
	DirectX::XMMATRIX GetMatrix() const noexcept;			

	// Получение данных о проекции камеры
	ProjectionData    GetProjectionData() const noexcept;	

	// Получение данных о проекции камеры в виде матрицы
	DirectX::XMMATRIX GetProjection() const noexcept;

	// Перегруженный метод для установки позиции камеры, вклюячая её индиктор и проекцию
	void			  SetPosition(DirectX::XMFLOAT3 position) noexcept;

	void SetTravelSpeed(float tSpeed);

	// Отрисовать базовый интерфейс управления
	void SpawnDefaultControl(Graphics& gfx) noexcept;
private:
	// Режим ориентации камеры
	bool tethered;					

	// Исходная позиция камеры.
	// Не путать с Object.position: представляет собой копию данных
	// для сброса настроек
	DirectX::XMFLOAT3 homePosition;					

	// Исходная ориентация камеры.
	// Не путать с Object.orientation: представляет собой копию данных
	// для сброса настроек
	DirectX::XMFLOAT3 homeOrientation;

	// Скорость перемещения камеры по сцене
	float travelSpeed = 24.0f;	

	// Скорость вращения камеры на сцене
	float rotationSpeed = 0.004f;
private:
	// Проекция камеры.
	// Не путать с struct ProjectionData: описывает функционал проекции,
	// а не набор данных для неё
	Projection proj;

	// Видимость индикатора камеры
	bool enableCameraIndicator = false;		

	// Видимость проекции камеры
	bool enableFrustumIndicator = false;					

	// Индикатор камеры
	CameraIndicator indicator;
};
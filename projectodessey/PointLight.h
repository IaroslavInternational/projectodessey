#pragma once

#include "Graphics.h"
#include "SolidSphere.h"
#include "ConstantBuffers.h"
#include "ConditionalNoexcept.h"

namespace Rgph
{
	class RenderGraph;
}

class Camera;

struct PointLightCBuf	// Буфер данных ист. освещения для шейдера
{
	alignas(16) DirectX::XMFLOAT3 pos;
	alignas(16) DirectX::XMFLOAT3 ambient;
	alignas(16) DirectX::XMFLOAT3 diffuseColor;
	float diffuseIntensity;
	float attConst;
	float attLin;
	float attQuad;
};

class PointLight
{
public:
	friend class GUISystem;
public:
	PointLight(std::string path, Graphics& gfx);
public:
	void LinkTechniques(Rgph::RenderGraph& rg);							// Добавить к рендеру
	void Submit(size_t channels) const noxnd;							// Добавить к каналу отрисовки
	void Bind(Graphics& gfx, DirectX::FXMMATRIX view) const noexcept;	// Обновить данные для шейдера
public:
	std::string GetName() const noexcept;								// Получить имя ист. света
	PointLightCBuf GetData() const noexcept;							// Получить данные ист. освещения
	void Reset() noexcept;												// Сбросить данные
	std::shared_ptr<Camera> ShareCamera() const noexcept;				// Получить укзатель на камеру
private:
	PointLightCBuf home;												// Исходный буфер данных
	PointLightCBuf cbData;												// Изменяемый буфер данных
	mutable SolidSphere mesh;											// Меш (сфера)
	mutable Bind::PixelConstantBuffer<PointLightCBuf> cbuf;				// Буфер шейдера
	mutable std::shared_ptr<Camera> pCamera;							// Указатель на камеру
private:
	bool IsRendered = true;												// Разрешена ли отрисовка
};
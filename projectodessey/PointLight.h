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

struct PointLightCBuf	// ����� ������ ���. ��������� ��� �������
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
	void LinkTechniques(Rgph::RenderGraph& rg);							// �������� � �������
	void Submit(size_t channels) const noxnd;							// �������� � ������ ���������
	void Bind(Graphics& gfx, DirectX::FXMMATRIX view) const noexcept;	// �������� ������ ��� �������
public:
	std::string GetName() const noexcept;								// �������� ��� ���. �����
	PointLightCBuf GetData() const noexcept;							// �������� ������ ���. ���������
	void Reset() noexcept;												// �������� ������
	std::shared_ptr<Camera> ShareCamera() const noexcept;				// �������� �������� �� ������
private:
	PointLightCBuf home;												// �������� ����� ������
	PointLightCBuf cbData;												// ���������� ����� ������
	mutable SolidSphere mesh;											// ��� (�����)
	mutable Bind::PixelConstantBuffer<PointLightCBuf> cbuf;				// ����� �������
	mutable std::shared_ptr<Camera> pCamera;							// ��������� �� ������
private:
	bool IsRendered = true;												// ��������� �� ���������
};
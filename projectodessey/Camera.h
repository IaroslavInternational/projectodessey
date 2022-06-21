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

// ����� ��� �������� ����������� �����. 
class Camera : public VisibleObject
{
public:
	friend class GUISystem;
public:
	// ����������� ������ ������� ��������� ����:
	// name - ��� ������
	// gfx - ����� ������������ ����
	// position - ������� ������
	// orientation - ���������� ������
	// prd - ������ � �������� ������
	// visibility - ��������� ������
	// tethered - ����� ���������� ������
	Camera(std::string		 name,
		   Graphics&		 gfx,
		   DirectX::XMFLOAT3 position =	   { 0.0f, 0.0f, 0.0f },
		   DirectX::XMFLOAT3 orientation = { 0.0f, 0.0f, 0.0f },
		   ProjectionData	 prd =		   { 1.0f, 9.0f / 16.0f, 0.5f, 400.0f },
		   bool				 visibility =  true,   
		   bool				 tethered =	   false) noexcept;
public:
	// ����� ��� ������������� ������ � Render Graph
	void			  LinkTechniques(Rgph::RenderGraph& rg);

	// ����� ��� ������������� ������ � ������ ���������
	void			  Submit(size_t channel);		

	// ����� ��� ���������� ������ � ���� �������
	void			  BindToGraphics(Graphics& gfx) const;				

	// ����� �������� ������ � ���������,
	// gfx ����� ��� ����������� ���������� � ��������
	void			  Reset(Graphics& gfx) noexcept;

	// �������� ������
	void			  Rotate(float dx, float dy) noexcept;

	// ����������� ������
	void			  Translate(DirectX::XMFLOAT3 translation) noexcept;

	// ��������� ������� ������
	DirectX::XMMATRIX GetMatrix() const noexcept;			

	// ��������� ������ � �������� ������
	ProjectionData    GetProjectionData() const noexcept;	

	// ��������� ������ � �������� ������ � ���� �������
	DirectX::XMMATRIX GetProjection() const noexcept;

	// ������������� ����� ��� ��������� ������� ������, �������� � �������� � ��������
	void			  SetPosition(DirectX::XMFLOAT3 position) noexcept;

	void SetTravelSpeed(float tSpeed);

	// ���������� ������� ��������� ����������
	void SpawnDefaultControl(Graphics& gfx) noexcept;
private:
	// ����� ���������� ������
	bool tethered;					

	// �������� ������� ������.
	// �� ������ � Object.position: ������������ ����� ����� ������
	// ��� ������ ��������
	DirectX::XMFLOAT3 homePosition;					

	// �������� ���������� ������.
	// �� ������ � Object.orientation: ������������ ����� ����� ������
	// ��� ������ ��������
	DirectX::XMFLOAT3 homeOrientation;

	// �������� ����������� ������ �� �����
	float travelSpeed = 24.0f;	

	// �������� �������� ������ �� �����
	float rotationSpeed = 0.004f;
private:
	// �������� ������.
	// �� ������ � struct ProjectionData: ��������� ���������� ��������,
	// � �� ����� ������ ��� ��
	Projection proj;

	// ��������� ���������� ������
	bool enableCameraIndicator = false;		

	// ��������� �������� ������
	bool enableFrustumIndicator = false;					

	// ��������� ������
	CameraIndicator indicator;
};
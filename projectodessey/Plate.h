#pragma once

#include "Drawable.h"
#include "IndexedTriangleList.h"

// ������������ ����� ����������� ������ �� ����� � ���� �� ��������� ���������� ���������,
// ������� ������. ������������ � ������ Trigger ��� ����������� ������� ��������������� �������� ��������
class Plate : public Drawable
{
public:
	friend class GUISystem;
public:
	// ����������� ���������� ������� ��������� ����:
	// gfx - ����� ������������ ����
	// position - ������� ���������
	// orientation - ���������� ���������
	Plate(Graphics&		    gfx, 
		  DirectX::XMFLOAT2 size, 
		  DirectX::XMFLOAT3 position =	  { 0.0f, 0.0f, 0.0f },
		  DirectX::XMFLOAT3 orientation = { 0.0f, 0.0f, 0.0f},
		  DirectX::XMFLOAT4 color =		  { 0.0f, 192.0f, 204.0f, 0.0f });
public:
	// ���������� ������
	// size.x - ������,
	// size.y - ������
	void			  SetSize(DirectX::XMFLOAT2 size);

	// ���������� ������
	void			  SetPosition(DirectX::XMFLOAT3 position) noexcept;

	// ���������� ����������
	void			  SetRotation(DirectX::XMFLOAT3 orientation) noexcept;

	void			  UpdateSize(DirectX::XMFLOAT2 size);

	// �������� ������
	// size.x - ������,
	// size.y - ������
	DirectX::XMFLOAT2 GetSize() const noexcept;

	// �������� ��������� �� ������
	// size.x - ������,
	// size.y - ������
	DirectX::XMFLOAT2* GetSizePtr() noexcept;

	// �������� ������� ������������� ���������
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	void			  CreatePlate();
private:
	Graphics& gfx;

	// ���� �������
	struct PSMaterialConstant
	{
		DirectX::XMFLOAT4 color;
	} pmc;

	// ������ ���������
	DirectX::XMFLOAT2 size;

	// ������� ���������
	DirectX::XMFLOAT3 position;

	// ���������� ���������
	DirectX::XMFLOAT3 orientation;

	IndexedTriangleList model;
};
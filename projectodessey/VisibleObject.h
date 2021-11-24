#pragma once

#include "Object.h"

namespace Rgph
{
	class RenderGraph;
}

// ������� ����� ��� ������� �������� �� �����. � ������� �� Object ����� ���������� ������ ��� ���������.
class VisibleObject : public Object
{
public:
	// ����������� �������� ������� ������� ��������� ����:
	// name - ��� �������
	// position - ������� �������
	// orientation - ���������� �������
	// visibility - ��������� �������
	VisibleObject(std::string name, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 orientation, bool visibility);

	// ����������� ����������
	virtual ~VisibleObject() {};
public:
	// ����������� ����� ��� ������������� ������� � Render Graph
	virtual void LinkTechniques(Rgph::RenderGraph& rg) = 0;

	// ����������� ����� ��� ������������� ������� � ������ ���������
	virtual void Submit(size_t channels) = 0;

	// ���������� ��������� �������
	void		 SetVisibility(bool visibility);

	// �������� ��������� �������
	bool		 IsVisible() const;
protected:
	// ��������� �������
	bool visibility;
};
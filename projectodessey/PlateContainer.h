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
	void LinkTechniques();												// �������� � �������
	void Submit(size_t channels);										// �������� � ������ ���������
	bool CheckCollision(HitBox& hb);
private:
	std::string							path;							// ���� � ����� � �������
	Graphics&							gfx;							// ����� ������������ ����
	Rgph::RenderGraph&					rg;								// ����� ������-�����
	std::vector<std::unique_ptr<Plate>> plates;							// ��������� �� ���������
private:
	std::string selected = "";											// ��������� ������� � ������ �������
};


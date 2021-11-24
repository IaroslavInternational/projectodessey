#pragma once

#include "Model.h"

class ModelContainer
{
public:
	friend class GUISystem;
public:
	ModelContainer(std::string path, Graphics& gfx, Rgph::RenderGraph& rg);
public:
	void LinkTechniques();												// �������� � �������
	void Submit(size_t channels);										// �������� � ������ ���������
public:
	std::string GetModelNameByIndex(size_t index);						// ��� ������ �� �������
public:
//	void OpenDialog();													// ������� ���������� ���� ��� ���������� ������ �� �����
	size_t ModelsAmount();												// ���-�� �������
private:
	void LoadModel(std::string name, std::string path);					// �������� ������
	void DeleteModel(std::string name);									// ������� ������
	void Init();														// ���������� ��������� � ���������� ������
private:
	std::string path;													// ���� � ����� � ������� � �������
	Graphics& gfx;														// ����� ������������ ����
	Rgph::RenderGraph& rg;												// ����� ������-�����
	std::vector<std::unique_ptr<Model>> models;							// ��������� �� ������
private:
	std::string selected = "";											// ��������� ������� � ������ �������
	bool IsSave = false;												// ���� ������ ������ ���������
	bool IsDelete = false;												// ���� ������ ������ �������
};
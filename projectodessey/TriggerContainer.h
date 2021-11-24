#pragma once

#include "Trigger.h"

#include <memory>
#include <vector>

// �������� ��� �������� �� ��������� �����
class TriggerContainer
{
public:
	friend class GUISystem;
public:
	TriggerContainer(std::string path, Graphics& gfx, Rgph::RenderGraph& rg);
public:
	void LinkTechniques();
	void Submit(size_t channels);
public:
	std::pair<std::string, bool> Check(dx::XMFLOAT3 pos); // ��� ����, ������ | �������� �� ����������� ��������
private:
	void AddTrigger(std::string name, std::string ptr); //
	void DeleteTrigger(std::string name);				// ������� ������
private:
	std::string path;								// ���� � ����� � ������� � �������
	Graphics& gfx;									// ����� ������������ ����
	Rgph::RenderGraph& rg;							// ����� ������-�����
	std::vector<std::unique_ptr<Trigger>> triggers;	// ��������
};
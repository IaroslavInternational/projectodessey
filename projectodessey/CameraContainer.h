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
	Camera* operator->();											// ��������� �� ������
	void LinkTechniques();
	void Submit(size_t channels) const;								// �������� � ������ ���������
	void Bind();													// ��������� ������ � ����������� ����
public:
	Camera& GetActiveCamera();
public:
	size_t CamerasAmount();											// ���-�� �����
	std::string GetCameraNameByIndex(size_t index);					// ��� ������ �� ������� � ����������
public:
	void ShowLeftPanel();											// �������� ����� ������ ��� �����
	void ShowRightPanel();											// �������� ������ ������ ��� �����
	void AddCamera(std::shared_ptr<Camera> pCam);					// �������� ������
	void AddRuntimeCamera(std::shared_ptr<Camera> pCam);			// �������� ������ � �������� � ����
private:
	void DeleteCamera(std::string name);							// ������� ������
	Camera& GetControlledCamera();									// ����� ����������� ������
private:
	std::string path = "";											// ���� � ����� � ������� � �������
	Graphics& gfx;													// ����� ������������ ����
	Rgph::RenderGraph& rg;											// ����� ������-�����
	std::vector<std::shared_ptr<Camera>> cameras;					// ������
private:
	size_t active     = 0;											// ������ �������� ������
	size_t controlled = 0;											// ������ ����������� ������
};
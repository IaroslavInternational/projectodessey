#pragma once

#include "Scene.h"
#include "SceneObjects.h"
#include <map>

class GUISystem
{
public:
	GUISystem(Scene* scene);
public:
	void Show();
	void Hide();
private:
	void SetPanelWidthAndPos(int corner, float width, float height, float x_offset = 0.0f, float y_offset = 0.0f);
	void ShowMenu();			// ������ ������ ����
	void ShowLeftPanel();		// ����� ������
	void ShowRightPanel();		// ������ ������	
	void ShowLeftBottomPanel();	// ����� ������ ������
	void ShowBottomPanel();		// ������ ������
	void ShowFPSAndGPU();		// �������� FPS � ����������� �������� *����� ������ ������*
	void ShowOptionalPanel();	// 
	void ShowLog();				// �������� ��� *������ ������*
	void DisableSides();		// ��������� ������� ������
	void SetGuiColors();		// �������� ����� ImGui
private:
	void ShowModelsList();		//
	void ShowTriggersList();	//
	void ShowCamerasList();		//
								
	void ShowModelsControl();	//
	void ShowTriggersControl();	//
	void ShowCamerasControl();	//
								
	void ShowPLightControl();	//

	void ShowRobotControl();

	void LoadModel(std::string name, std::string path);
	void OpenDialog();
private:
	bool ShowModelsSettings   = true;   //
	bool ShowTriggersSettings = false;  // ������ ������ ��������� �� �����
	bool ShowCamerasSettings  = false;  // ����� ������ ����� �� ����� � �����������
	bool ShowPLightSettings   = false;  // ������ ������ ���������� ����������� �������� ���������� �����
	bool ShowRobotSettings	  = true;	//
	bool ShowHardwareInfo     = true;  // FPS � ���������� � ����������� ��������
	bool ShowLogs		      = true;  // ���
private:
	AppLog			  applog;	// ���
	PointLight*       pLight;
	ModelContainer*   models;
	CameraContainer*  cameras;
	TriggerContainer* triggers;
	Robot*			  rb;
	SimulationModel*  sim;
private:
	bool						   IsShow   = true;
	bool						   IsSave   = false;
	bool						   IsDelete = false;
	bool						   IsAdd    = false;
	std::map<std::wstring, double> gpu_desc;	// ��������� �������� ����������� ��������� [first - ���, second - ����� ������]
private:
	/* ��������������� ���������� ��� ������ � �������� */

	std::string mSelected = "";

	/****************************************************/
private:
	/* ��������������� ���������� ��� ������ � �������� */

	std::string cSelected = "";

	/****************************************************/
private:
	/* ��������������� ���������� ��� ������ � ��������� */

	std::string tSelected = "";

	/*****************************************************/
};
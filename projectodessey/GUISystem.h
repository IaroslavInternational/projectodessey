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
	void ShowMenu();			// Верхня панель меню
	void ShowLeftPanel();		// Левая панель
	void ShowRightPanel();		// Правая панель	
	void ShowLeftBottomPanel();	// Левая нижняя панель
	void ShowBottomPanel();		// Нижняя панель
	void ShowFPSAndGPU();		// Показать FPS и графические адаптеры *Левая нижняя панель*
	void ShowOptionalPanel();	// 
	void ShowLog();				// Показать лог *Нижняя панель*
	void DisableSides();		// Отключить боковые панели
	void SetGuiColors();		// Цветовая гамма ImGui
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
	bool ShowTriggersSettings = false;  // Правая панель триггеров на сцене
	bool ShowCamerasSettings  = false;  // Левая панель камер на сцене с настройками
	bool ShowPLightSettings   = false;  // Правая панель управления настройками точечных источников света
	bool ShowRobotSettings	  = true;	//
	bool ShowHardwareInfo     = true;  // FPS и информация о графическом адаптере
	bool ShowLogs		      = true;  // Лог
private:
	AppLog			  applog;	// Лог
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
	std::map<std::wstring, double> gpu_desc;	// Контейнер описаний графических адаптеров [first - имя, second - объём памяти]
private:
	/* Вспомогательные переменные для работы с моделями */

	std::string mSelected = "";

	/****************************************************/
private:
	/* Вспомогательные переменные для работы с камерами */

	std::string cSelected = "";

	/****************************************************/
private:
	/* Вспомогательные переменные для работы с тригерами */

	std::string tSelected = "";

	/*****************************************************/
};
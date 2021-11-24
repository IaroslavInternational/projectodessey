#pragma once

#include "ImguiManager.h"
#include "Window.h"
#include "EngineTimer.h"
#include "ScriptCommander.h"
#include "Scene.h"
#include "GUISystem.h"

class App
{
public:
	App(const std::string& commandLine = "");
public:
	int Go();	// Игровой цикл
private:
	void HandleInput(float dt);	// Обработка данных переферии
	void DoFrame(float dt);		// Отрисовка кадра
private:
	ImguiManager  imgui;  //
private:
	std::string                commandLine;		    // Коммандная строка
	std::shared_ptr<Window>    wnd;				    // Указатель на окно	
	ScriptCommander			   scriptCommander;	    // Управление скриптами	
	EngineTimer				   timer;				// Игровой таймер	
	float					   speed_factor = 1.0f; // Скорость
	std::unique_ptr<Scene>     scene;			    // Сцена
	std::shared_ptr<GUISystem> gui;					//
};
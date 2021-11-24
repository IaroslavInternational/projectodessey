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
	int Go();	// ������� ����
private:
	void HandleInput(float dt);	// ��������� ������ ���������
	void DoFrame(float dt);		// ��������� �����
private:
	ImguiManager  imgui;  //
private:
	std::string                commandLine;		    // ���������� ������
	std::shared_ptr<Window>    wnd;				    // ��������� �� ����	
	ScriptCommander			   scriptCommander;	    // ���������� ���������	
	EngineTimer				   timer;				// ������� ������	
	float					   speed_factor = 1.0f; // ��������
	std::unique_ptr<Scene>     scene;			    // �����
	std::shared_ptr<GUISystem> gui;					//
};
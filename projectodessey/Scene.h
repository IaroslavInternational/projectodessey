#pragma once

#include "Window.h"
#include "BlurOutlineRenderGraph.h"
#include "EngineTimer.h"
#include "SceneObjects.h"
#include "Robot.h"
#include "SimulationModel.h"

class Scene
{
public:
	friend class GUISystem;
public:
	Scene(std::shared_ptr<Window> wnd, std::string data);
public:
	void						 ProcessInput(float dt); // ��������� ������� ������
	void						 Render(float dt);       // ��������� �����
private:
	bool cursorState = false;
private:
	std::shared_ptr<Window>		 wnd;				// ��������� �� ������� ���� 
	Rgph::BlurOutlineRenderGraph rg{ wnd->Gfx() };	// ������ ����
	SceneObjects				 objects;
	Robot						 robot;
	SimulationModel				 sim;
	std::unique_ptr<Model>	     axis;	
};
#pragma once

#include "ModelContainer.h"
#include "CameraContainer.h"
#include "PointLight.h"
#include "PlateContainer.h"
#include "AppLog.h"
#include "SceneDataReader.h"
#include "BlurOutlineRenderGraph.h"

class SceneObjects
{
public:
	SceneObjects(std::string pathToObjectsData, Graphics& gfx, Rgph::BlurOutlineRenderGraph& rg);
public:
	void LinkTechniques(Rgph::RenderGraph& rg);
	void Submit(size_t channels);
private:
	SceneDataReader sdr;    // ����������� ����� � ������ � ������� ��� �����������	
public:
	/***** -�������- *****/
	
	CameraContainer cameras; // ��������� �����
	PointLight	    pLight;  // �������� �����
	ModelContainer  models;  // ������ �����
	PlateContainer  plates;

	/*********************/
};
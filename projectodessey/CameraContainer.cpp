#include "CameraContainer.h"
#include "Camera.h"
#include "Graphics.h"
#include "RenderGraph.h"

#include "EngineFunctions.hpp"

CameraContainer::CameraContainer(std::string path, Graphics& gfx, Rgph::RenderGraph& rg)
	:
	path(path),
	gfx(gfx),
	rg(rg)
{
	std::ifstream dataFile(path);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными о камерах");
	}

	json j;
	dataFile >> j;

	dataFile.close();

	ProjectionData prd;

	for (json::iterator m = j.begin(); m != j.end(); ++m)
	{
		auto& d = m.key();

		for (const auto& obj : j.at(d))
		{
			/* Получение имени объекта */

			std::string name = obj.at("name");
			std::transform(name.begin(), name.end(), name.begin(), tolower);

			/************************/

			/* Получение позиции */

			float pos_x = obj.at("pos-x");
			float pos_y = obj.at("pos-y");
			float pos_z = obj.at("pos-z");

			DirectX::XMFLOAT3 position = { pos_x, pos_y, pos_z };

			/******************/

			/* Получение ориентации */

			float pitch = obj.at("pitch");
			float yaw = obj.at("yaw");

			DirectX::XMFLOAT3 orientation = { pitch, yaw, 0.0f };

			/*********************/

			/* Получение проекции */

			float proj_w = obj.at("proj-width");
			float proj_h = obj.at("proj-height");
			float proj_nZ = obj.at("proj-nearZ");
			float proj_fZ = obj.at("proj-farZ");

			prd = { proj_w, proj_h, proj_nZ, proj_fZ };

			/**********************/

			/* Инициализация камеры */

			cameras.emplace_back(std::make_shared<Camera>(name, gfx, position, orientation, prd));

			/************************/
		}
	}

	cameras.shrink_to_fit();
}

Camera* CameraContainer::operator->()
{
	return &GetActiveCamera();
}

void CameraContainer::LinkTechniques()
{
	for(int i = 0; i < cameras.size(); i++)
	{
		cameras[i]->LinkTechniques(rg);
	}
}

void CameraContainer::Submit(size_t channels) const
{
	for( size_t i = 0; i < cameras.size(); i++ )
	{
		if( i != active )
		{
			cameras[i]->Submit( channels );
		}
	}
}

void CameraContainer::Bind()
{
	gfx.SetCamera((*this)->GetMatrix());
}

Camera& CameraContainer::GetActiveCamera()
{
	return *cameras[active];
}

size_t CameraContainer::CamerasAmount()
{
	return cameras.size();
}

std::string CameraContainer::GetCameraNameByIndex(size_t index)
{
	return cameras.at(index)->GetName().c_str();
}

void CameraContainer::AddCamera(std::shared_ptr<Camera> pCam)
{
	cameras.push_back(std::move(pCam));
}

void CameraContainer::AddRuntimeCamera(std::shared_ptr<Camera> pCam)
{
	using std::to_string;

	// Открытие файла с данными о моделях
	std::ifstream dataFile(this->path);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными о камерах сцены");
	}

	// Чтение файла
	json j;
	dataFile >> j;

	// Закрытие файла
	dataFile.close();

	// Новая камера
	std::ostringstream newCam;
	newCam << "\"" << pCam->GetName() << "\":[{";

	newCam << "\"name\": \"" << pCam->GetName() << "\",";

	newCam << "\"pos-x\": " <<  pCam->GetPosition().x << ",";
	newCam << "\"pos-y\" : " << pCam->GetPosition().y << ",";
	newCam << "\"pos-z\" : " << pCam->GetPosition().z << ",";

	newCam << "\"proj-farZ\" : " << pCam->GetProjectionData().farZ << ",";
	newCam << "\"proj-height\" : " << pCam->GetProjectionData().height << ",";
	newCam << "\"proj-nearZ\" : " << pCam->GetProjectionData().nearZ << ",";
	newCam << "\"proj-width\" : " << pCam->GetProjectionData().width << ",";

	newCam << "\"pitch\": " << pCam->GetOrientation().x << ",";
	newCam << "\"yaw\": " << pCam->GetOrientation().y << "}]";

	// Подготовка к вставке в файл
	std::string json_str = j.dump();
	size_t pos_of_par = json_str.find_last_of('}');
	size_t pos_of_par2 = json_str.find_last_of(']');

	json_str.at(pos_of_par) = ' ';
	json_str.at(pos_of_par2 + 1) = ',';

	// Запись в файл данных новой модели
	std::ofstream ostream(this->path);
	ostream << json_str + newCam.str() + '}';

	// Закрытие файла
	ostream.close();

	pCam->LinkTechniques(rg);

	AddCamera(pCam);
}

void CameraContainer::DeleteCamera(std::string name)
{
	for (auto cam = cameras.begin(); cam != cameras.end(); ++cam)
	{
		if (cam->get()->GetName() == name)
		{
			cameras.erase(cam);
			EngineFunctions::DeleteJsonObject(name, path);

			break;
		}
	}

	active = 0;
	controlled = 0;
}

Camera& CameraContainer::GetControlledCamera()
{
	return *cameras[controlled];
}
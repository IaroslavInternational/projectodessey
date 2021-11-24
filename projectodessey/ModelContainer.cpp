#include "ModelContainer.h"

#include "EngineFunctions.hpp"

#include "TestModelProbe.h"

ModelContainer::ModelContainer(std::string path, Graphics& gfx, Rgph::RenderGraph& rg)
	:
	path(path),
	gfx(gfx),
	rg(rg)
{
	std::ifstream dataFile(path);
	if (!dataFile.is_open())
	{
		throw ("�� ������ ������� ���� � ������� � �������");
	}

	json j;
	dataFile >> j;
	
	dataFile.close();

	for (json::iterator m = j.begin(); m != j.end(); ++m)
	{
		auto d = m.key();

		for (const auto& obj : j.at(d))
		{
			/* ��������� ����� ������� */

			std::string name = obj.at("name");
			std::transform(name.begin(), name.end(), name.begin(), tolower);

			/************************/

			/* ��������� ������� */

			float pos_x = obj.at("pos-x");
			float pos_y = obj.at("pos-y");
			float pos_z = obj.at("pos-z");

			DirectX::XMFLOAT3 position = { pos_x, pos_y, pos_z };

			/******************/

			/* ��������� ���������� */

			float roll = obj.at("roll");
			float pitch = obj.at("pitch");
			float yaw = obj.at("yaw");

			DirectX::XMFLOAT3 orientation = { roll, pitch, yaw };

			/*********************/

			/* ��������� ���� � ������ */

			std::string modelPath = obj.at("path");

			/***************************/

			/* ��������� ��������� ������� */

			float modelScale = obj.at("scale");

			/*******************************/

			/* ������������� ������ */

			models.emplace_back(std::make_unique<Model>(name, modelPath, gfx, position, orientation, modelScale));

			/**************************************************/
		}
	}

	models.shrink_to_fit();

	Init();
}

void ModelContainer::LinkTechniques()
{
	for (int i = 0; i < models.size(); i++)
	{
		models[i]->LinkTechniques(rg);
	}
}

void ModelContainer::Submit(size_t channels)
{
	for (auto& m : models)
	{
		m->Submit(channels);
	}
}

std::string ModelContainer::GetModelNameByIndex(size_t index)
{
	return models[index]->GetName();
}

void ModelContainer::LoadModel(std::string name, std::string path)
{
	models.emplace_back(std::make_unique<Model>(name, path, gfx));
	models.back()->LinkTechniques(rg);

	using std::to_string;

	// �������� ����� � ������� � �������
	std::ifstream dataFile(this->path);
	if (!dataFile.is_open())
	{
		throw ("�� ������ ������� ���� � ������� � ������� ����");
	}

	// ������ �����
	json j;
	dataFile >> j;

	// �������� �����
	dataFile.close();

	// ����� ������
	std::ostringstream newModel;
	newModel << "\"" << name << "\":[{";

	newModel << "\"name\": \""  << name << "\",";
	newModel << "\"path\": \""  << path << "\",";
	newModel << "\"scale\": " << 1.0f << ",";

	newModel << "\"pos-x\": "  << 0.0f << ",";
	newModel << "\"pos-y\" : " << 0.0f << ",";
	newModel << "\"pos-z\" : " << 0.0f << ",";

	newModel << "\"roll\": "  << 0.0f << ",";
	newModel << "\"pitch\": " << 0.0f << ",";
	newModel << "\"yaw\": "   << 0.0f << "}]";

	// ���������� � ������� � ����
	std::string json_str = j.dump();
	size_t pos_of_par = json_str.find_last_of('}');
	size_t pos_of_par2 = json_str.find_last_of(']');

	json_str.at(pos_of_par) = ' ';
	json_str.at(pos_of_par2 + 1) = ',';

	// ������ � ���� ������ ����� ������
	std::ofstream ostream(this->path);
	ostream << json_str + newModel.str() + '}';

	// �������� �����
	ostream.close();
}

void ModelContainer::DeleteModel(std::string name)
{
	for (auto model = models.begin(); model != models.end(); ++model)
	{
		if (model->get()->GetName() == name)
		{
			models.erase(model);
			EngineFunctions::DeleteJsonObject(name, path);

			break;
		}
	}
}

void ModelContainer::Init()
{
	for (int i = 0; i < models.size(); i++)
	{
		models[i]->SetRootTransform
		(
			dx::XMMatrixRotationX(models[i]->GetOrientation().x) *
			dx::XMMatrixRotationY(models[i]->GetOrientation().y) *
			dx::XMMatrixRotationZ(models[i]->GetOrientation().z) *
			dx::XMMatrixTranslation(
				models[i]->GetPosition().x,
				models[i]->GetPosition().y,
				models[i]->GetPosition().z
			)
		);
	}
}
/*
void ModelContainer::OpenDialog()
{
	if (ImGuiFileDialog::Instance()->Display("ModelOD"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			std::string copy1 = filePathName;

			std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			std::string copy2 = filePath;

			std::string name = copy1.replace(0, copy2.size() + 1, "");

			for (int i = 0; i != filePathName.size(); i++)
			{
				if (filePathName[i] == '\\')
				{
					filePathName[i] = '/';
				}
			}

			LoadModel(name.c_str(), filePathName.c_str());
		}

		ImGuiFileDialog::Instance()->Close();
	}
}
*/
size_t ModelContainer::ModelsAmount()
{
	return models.size();
}
#include "TriggerContainer.h"

#include "EngineFunctions.hpp"

TriggerContainer::TriggerContainer(std::string path, Graphics& gfx, Rgph::RenderGraph& rg)
	:
	path(path),
	gfx(gfx),
	rg(rg)
{
	std::ifstream dataFile(path);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными о триггерах сцен");
	}

	DirectX::XMFLOAT2 size;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 orientation;

	json j;
	dataFile >> j;

	for (json::iterator t = j.begin(); t != j.end(); ++t)
	{
		auto &d = t.key();

		for (const auto& obj : j.at(d))
		{
			/* Запись положения триггера */

			position.x = obj.at("pos-x");
			position.y = obj.at("pos-y");
			position.z = obj.at("pos-z");

			orientation.x = obj.at("roll");
			orientation.y = obj.at("pitch");
			orientation.z = obj.at("yaw");

			/***************************/

			/* Запись размера триггера */
			
			size.x = obj.at("width");
			size.y = obj.at("height");

			/***************************/

			/* Запись указателя и имени триггера */

			std::string ptr = obj.at("ptr");

			std::string name = obj.at("name");

			/*****************************/ 

			triggers.push_back(std::make_unique<Trigger>(name, gfx, size, position, orientation));
		}
	}

	dataFile.close();
}

void TriggerContainer::LinkTechniques()
{
	for (auto& t : triggers)
	{
		t->LinkTechniques(rg);
		t->SetDefault();
	}
}

void TriggerContainer::Submit(size_t channels)
{
	for (auto& t : triggers)
	{
		t->Submit(channels);
	}
}

std::pair<std::string, bool> TriggerContainer::Check(DirectX::XMFLOAT3 position)
{
	for (auto& t : triggers)
	{
		if (t->Check(position))
		{
			auto HittedTriggerGoal = "GOAL HERE"; // !!!

			return std::make_pair(HittedTriggerGoal, true);
		}
	}

	return std::make_pair("NULL", false);
}

void TriggerContainer::AddTrigger(std::string name, std::string ptr)
{
	using std::to_string;

	// Открытие файла с триггерами
	std::ifstream dataFile(path);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными о триггерах сцен");
	}

	// Чтение файла
	json j;
	dataFile >> j;

	// Закрытие файла
	dataFile.close();

	// Число триггеров в файле + 1
	size_t triggerIndex = triggers.size() + 1;

	// Новый триггер
	std::ostringstream newTrigger;
	/*newTrigger << "\"Trigger " << triggerIndex;
	
	newTrigger << "\": [{\"name\": \"" + name + "\",";

	newTrigger << "\"pos-lt\" :";
	newTrigger << "[{\"pos-x\": " << trs.PosTopLeft.x << ",";
	newTrigger << "\"pos-y\" : " << trs.PosTopLeft.y << ",";
	newTrigger << "\"pos-z\" : " << trs.PosTopLeft.z << "}],";

	newTrigger << "\"pos-rt\" :";
	newTrigger << "[{\"pos-x\": " << trs.PosTopRight.x << ",";
	newTrigger << "\"pos-y\" : " << trs.PosTopRight.y << ",";
	newTrigger << "\"pos-z\" : " << trs.PosTopRight.z << "}],";

	newTrigger << "\"pos-lb\" :";
	newTrigger << "[{\"pos-x\": " << trs.PosBottomLeft.x << ",";
	newTrigger << "\"pos-y\" : " << trs.PosBottomLeft.y << ",";
	newTrigger << "\"pos-z\" : " << trs.PosBottomLeft.z << "}],";
	
	newTrigger << "\"pos-rb\" :";
	newTrigger << "[{\"pos-x\": " << trs.PosBottomRight.x << ",";
	newTrigger << "\"pos-y\" : " << trs.PosBottomRight.y << ",";
	newTrigger << "\"pos-z\" : " << trs.PosBottomRight.z << "}],";

	newTrigger << "\"ptr\": \"" << ptr << "\",";

	newTrigger << "\"roll\": " << trs.Roll << ",";
	newTrigger << "\"pitch\": " << trs.Pitch << ",";
	newTrigger << "\"yaw\": " << trs.Yaw << "}]";*/

	// Подготовка к вставке в файл
	std::string json_str = j.dump();
	size_t pos_of_par = json_str.find_last_of('}');
	size_t pos_of_par2 = json_str.find_last_of(']');

	json_str.at(pos_of_par) = ' ';
	json_str.at(pos_of_par2 + 1) = ',';

	// Запись в файл нового триггера
	std::ofstream ostr(path);
	ostr << json_str << newTrigger.str() << '}';

	// Закрытие файла
	ostr.close();

	//trig_sc_container.emplace(ptr, std::make_unique<Trigger>(name, trs, gfx));

	for (auto& t : triggers)
	{
		if (t->GetName() == name)
		{
			t->LinkTechniques(rg);
			t->SetDefault();
		}
	}
}
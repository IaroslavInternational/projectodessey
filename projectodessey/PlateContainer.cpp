#include "PlateContainer.h"
#include "EngineFunctions.hpp"

PlateContainer::PlateContainer(std::string path, Graphics& gfx, Rgph::RenderGraph& rg)
	:
	path(path),
	gfx(gfx),
	rg(rg)
{
	std::ifstream dataFile(path);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными о панелях");
	}

	json j;
	dataFile >> j;

	dataFile.close();

	for (json::iterator m = j.begin(); m != j.end(); ++m)
	{
		auto d = m.key();

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

			/* Получение позиции */

			float w = obj.at("width");
			float h = obj.at("height");

			DirectX::XMFLOAT2 size = { w, h };

			/******************/

			/* Получение ориентации */

			float roll = obj.at("roll");
			float pitch = obj.at("pitch");
			float yaw = obj.at("yaw");

			DirectX::XMFLOAT3 orientation = { roll, pitch, yaw };

			/*********************/

			/* Инициализация модели */

			plates.emplace_back(std::make_unique<Plate>(name, gfx, size, position, orientation));

			/************************/
		}
	}

	plates.shrink_to_fit();
}

void PlateContainer::LinkTechniques()
{
	for (auto& p : plates)
	{		
		p->LinkTechniques(rg);
	}
}

void PlateContainer::Submit(size_t channels)
{
	for (auto& p : plates)
	{
		if (p->IsDrawn())
		{
			p->Submit(channels);
		}
	}
}

bool PlateContainer::CheckCollision(HitBox& hb)
{
	for (auto& p : plates)
	{
		if (hb.IsCollide(*p.get()))
		{
			return true;
		}
	}
}

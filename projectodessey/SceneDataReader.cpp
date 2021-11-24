#include "SceneDataReader.h"

#include <sstream>
#include <fstream>
#include <filesystem>

#include "json.hpp"

SceneDataReader::SceneDataReader(std::string path)
{
	using json = nlohmann::json;
	using namespace std::string_literals;

	const auto dataPath = path;

	std::ifstream dataFile(dataPath);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными о сцене");
	}

	json j;
	dataFile >> j;

	for (json::iterator m = j.begin(); m != j.end(); ++m)
	{
		auto& d = m.key();

		for (const auto& obj : j.at(d))
		{
			/* Запись пути моделей */
			
			paths.emplace_back(obj.at("modelsPath"));

			/***********************/

			/* Запись пути триггеров */

			paths.emplace_back(obj.at("triggersPath"));

			/*************************/

			/* Запись пути триггеров */

			paths.emplace_back(obj.at("pLightsPath"));

			/*************************/

			/* Запись пути камер */

			paths.emplace_back(obj.at("camerasPath"));

			/*************************/
		}
	}
}

SceneDataReader::~SceneDataReader()
{
}

std::string SceneDataReader::GetCameraContainerPath() const
{
	return paths[3];
}

std::string SceneDataReader::GetPointLightPath() const
{
	return paths[2];
}

std::string SceneDataReader::GetModelContainerPath() const
{
	return paths[0];
}

std::string SceneDataReader::GetTriggerContainerPath() const
{
	return paths[1];
}

std::vector<std::string>& SceneDataReader::GetPaths()
{
	return paths;
}

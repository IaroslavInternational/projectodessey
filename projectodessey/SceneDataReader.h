#pragma once

#include <vector>
#include <string>

class SceneDataReader
{
public:
	SceneDataReader(std::string path);
	~SceneDataReader();
public:
	std::string GetCameraContainerPath() const;		// Путь к файлу с данными о контейнере камер
	std::string GetPointLightPath() const;			// Путь к файлу с данными о контейнере точ. ист. освещения
	std::string GetModelContainerPath() const;		// Путь к файлу с данными о контейнере моделей
	std::string GetTriggerContainerPath() const;	// Путь к файлу с данными о контейнере триггеров
private:
	// 0 - Путь к моделям
	// 1 - Путь к триггерам 
	// 2 - Путь к точечным источникам света
	// 3 - Путь к камерам
	std::vector<std::string>& GetPaths();
private:
	std::vector<std::string> paths;
};


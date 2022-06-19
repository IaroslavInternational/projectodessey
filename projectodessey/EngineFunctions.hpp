#pragma once

#include "AppLog.h"

#include <sstream>
#include <fstream>
#include <filesystem>
#include <random>
#include "json.hpp"

using json = nlohmann::json;
using namespace std::string_literals;

// Специальные функции для движка
namespace EngineFunctions
{
	/* 
	Установить новое значение для параметра в файле .json
	objectName - имя изменяемого объекта
	param - параметр для замены
	path - путь к файлу
	val - значение
	Applog - указатель на лог
	*/
	template<typename T>
	inline void static SetNewValue(std::string objectName, std::string param, T val, std::string path, AppLog* applog)
	{
		// Открытие файла
		std::ifstream dataFile(path);
		if (!dataFile.is_open())
		{
			throw ("Не удаётся открыть файл с данными");
		}

		if (applog != nullptr)
		{
			std::ostringstream ostrlog;
			ostrlog << "Установка [" << param << " : " << val << "] " << "для [" << objectName << "]\n";

			applog->AddLog(SYSTEM_LOG, ostrlog.str().c_str());
		}

		// Чтение файла
		json j;
		dataFile >> j;

		// Закрытие файла
		dataFile.close();

		for (json::iterator m = j.begin(); m != j.end(); ++m)
		{
			for (auto& obj : j.at(objectName))
			{
				obj[param] = val;
			}
		}

		// Запись в файл
		std::ofstream ostr(path);
		ostr << j.dump();

		// Закрытие файла
		ostr.close();
	}

	// Сохранить состояние объекта
	inline void static UpdateStatePattern(std::string objectName, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 orientation, std::string path, AppLog* applog)
	{
		SetNewValue<float>(objectName, "x", position.x, path, applog);
		SetNewValue<float>(objectName, "y", position.y, path, applog);
		SetNewValue<float>(objectName, "z", position.z, path, applog);
		SetNewValue<float>(objectName, "roll",  orientation.x, path, applog);
		SetNewValue<float>(objectName, "pitch", orientation.y, path, applog);
		SetNewValue<float>(objectName, "yaw",   orientation.z, path, applog);
	}

	// Заменить подстроку в строке
	inline std::string static StrReplace(const std::string& inputStr, const std::string& src, const std::string& dst)
	{
		std::string result(inputStr);

		size_t pos = result.find(src);

		while (pos != std::string::npos)
		{
			result.replace(pos, src.size(), dst);
			pos = result.find(src, pos);
		}

		return result;
	}

	// Удалить объект из файла json
	inline void static DeleteJsonObject(std::string objectName, std::string path)
	{
		// Открытие файла
		std::ifstream dataFile(path);
		if (!dataFile.is_open())
		{
			throw ("Не удаётся открыть файл с данными");
		}

		// Чтение файла
		json j;
		dataFile >> j;

		// Закрытие файла
		dataFile.close();

		if (j.size() - 1 != 0)
		{
			j[objectName] = "";

			auto j_str = j.dump();

			auto pos = j_str.find(objectName);

			if (j_str[pos + objectName.length() + 4] == '}')
			{
				for (auto i = pos - 2; i < pos + objectName.length() + 4; i++)
				{
					j_str[i] = '*';
				}
			}
			else
			{
				for (auto i = pos - 1; i < pos + objectName.length() + 5; i++)
				{
					j_str[i] = '*';
				}
			}

			j_str.erase(std::remove(j_str.begin(), j_str.end(), '*'), j_str.end());


			// Запись в файл
			std::ofstream ostr(path);
			ostr << j_str;

			// Закрытие файла
			ostr.close();
		}
		else
		{
			std::string str = "{}";

			// Запись в файл
			std::ofstream ostr(path);
			ostr << str;

			// Закрытие файла
			ostr.close();
		}
	}

	// Изменить имя объекта
	inline void static ChangeObjectName(const std::string& objectSrc, const std::string& objectDst, const std::string& path)
	{
		// Открытие файла
		std::ifstream dataFile(path);
		if (!dataFile.is_open())
		{
			throw ("Не удаётся открыть файл с данными");
		}

		// Чтение файла
		json j;
		dataFile >> j;

		// Закрытие файла
		dataFile.close();

		auto str = j.dump();
		str = EngineFunctions::StrReplace(str, objectSrc, objectDst);

		// Запись в файл
		std::ofstream ostr(path);
		ostr << str;

		// Закрытие файла
		ostr.close();
	}

	template <typename T>
	inline T static GenerateRandomNumber(T min, T max)
	{
		static std::random_device rd;
		static std::mt19937 rng{ rd() };

		if (typeid(T) == typeid(size_t))
		{
			static std::uniform_int_distribution<size_t> ud(min, max);

			return ud(rng);
		}
		else if (typeid(T) == typeid(int))
		{
			static std::uniform_int_distribution<int> ud(min, max);

			return ud(rng);
		}
		else if (typeid(T) == typeid(double))
		{
			static std::uniform_real_distribution<double> ud(min, max);

			return ud(rng);
		}
	}
}
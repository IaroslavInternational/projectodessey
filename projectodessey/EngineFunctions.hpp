#pragma once

#include "AppLog.h"

#include <sstream>
#include <fstream>
#include <filesystem>
#include <random>
#include "json.hpp"

using json = nlohmann::json;
using namespace std::string_literals;

// ����������� ������� ��� ������
namespace EngineFunctions
{
	/* 
	���������� ����� �������� ��� ��������� � ����� .json
	objectName - ��� ����������� �������
	param - �������� ��� ������
	path - ���� � �����
	val - ��������
	Applog - ��������� �� ���
	*/
	template<typename T>
	inline void static SetNewValue(std::string objectName, std::string param, T val, std::string path, AppLog* applog)
	{
		// �������� �����
		std::ifstream dataFile(path);
		if (!dataFile.is_open())
		{
			throw ("�� ������ ������� ���� � �������");
		}

		if (applog != nullptr)
		{
			std::ostringstream ostrlog;
			ostrlog << "��������� [" << param << " : " << val << "] " << "��� [" << objectName << "]\n";

			applog->AddLog(SYSTEM_LOG, ostrlog.str().c_str());
		}

		// ������ �����
		json j;
		dataFile >> j;

		// �������� �����
		dataFile.close();

		for (json::iterator m = j.begin(); m != j.end(); ++m)
		{
			for (auto& obj : j.at(objectName))
			{
				obj[param] = val;
			}
		}

		// ������ � ����
		std::ofstream ostr(path);
		ostr << j.dump();

		// �������� �����
		ostr.close();
	}

	// ��������� ��������� �������
	inline void static UpdateStatePattern(std::string objectName, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 orientation, std::string path, AppLog* applog)
	{
		SetNewValue<float>(objectName, "x", position.x, path, applog);
		SetNewValue<float>(objectName, "y", position.y, path, applog);
		SetNewValue<float>(objectName, "z", position.z, path, applog);
		SetNewValue<float>(objectName, "roll",  orientation.x, path, applog);
		SetNewValue<float>(objectName, "pitch", orientation.y, path, applog);
		SetNewValue<float>(objectName, "yaw",   orientation.z, path, applog);
	}

	// �������� ��������� � ������
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

	// ������� ������ �� ����� json
	inline void static DeleteJsonObject(std::string objectName, std::string path)
	{
		// �������� �����
		std::ifstream dataFile(path);
		if (!dataFile.is_open())
		{
			throw ("�� ������ ������� ���� � �������");
		}

		// ������ �����
		json j;
		dataFile >> j;

		// �������� �����
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


			// ������ � ����
			std::ofstream ostr(path);
			ostr << j_str;

			// �������� �����
			ostr.close();
		}
		else
		{
			std::string str = "{}";

			// ������ � ����
			std::ofstream ostr(path);
			ostr << str;

			// �������� �����
			ostr.close();
		}
	}

	// �������� ��� �������
	inline void static ChangeObjectName(const std::string& objectSrc, const std::string& objectDst, const std::string& path)
	{
		// �������� �����
		std::ifstream dataFile(path);
		if (!dataFile.is_open())
		{
			throw ("�� ������ ������� ���� � �������");
		}

		// ������ �����
		json j;
		dataFile >> j;

		// �������� �����
		dataFile.close();

		auto str = j.dump();
		str = EngineFunctions::StrReplace(str, objectSrc, objectDst);

		// ������ � ����
		std::ofstream ostr(path);
		ostr << str;

		// �������� �����
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
#include "PointLight.h"

#include "Camera.h"
#include "EngineMath.h"
#include "EngineFunctions.hpp"

PointLight::PointLight(std::string path, Graphics& gfx)
	:
	mesh(gfx, 0.5f),
	cbuf(gfx)
{
	std::ifstream dataFile(path);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными о точечном освещении");
	}

	json j;
	dataFile >> j;

	PointLightCBuf data;

	for (json::iterator m = j.begin(); m != j.end(); ++m)
	{
		auto& d = m.key();

		for (const auto& obj : j.at(d))
		{
			/* Запись имени объекта */

			std::string name = d.c_str();

			/************************/

			/* Запись позиции */

			float pos_x = obj.at("pos-x");
			float pos_y = obj.at("pos-y");
			float pos_z = obj.at("pos-z");

			data.pos = DirectX::XMFLOAT3(pos_x, pos_y, pos_z);

			/******************/

			/* Запись цветовых параметров */

			float ambient_r = obj.at("ambient-r");
			float ambient_g = obj.at("ambient-g");
			float ambient_b = obj.at("ambient-b");
			data.ambient = DirectX::XMFLOAT3(ambient_r, ambient_g, ambient_b);

			float diffuse_r = obj.at("diffuse-r");
			float diffuse_g = obj.at("diffuse-g");
			float diffuse_b = obj.at("diffuse-b");
			data.diffuseColor = DirectX::XMFLOAT3(diffuse_r, diffuse_g, diffuse_b);

			float diffuse_intensity = obj.at("diffuse-intensity");
			data.diffuseIntensity = diffuse_intensity;

			float att_const = obj.at("att-const");
			data.attConst = att_const;

			float att_lin = obj.at("att-lin");
			data.attLin = att_lin;

			float att_quad = obj.at("att-quad");
			data.attQuad = att_quad;

			/******************************/

			home = data;

			Reset();

			pCamera = std::make_shared<Camera>("Sun camera", gfx, cbData.pos);
			
			break;
		}
	}
}

void PointLight::LinkTechniques(Rgph::RenderGraph& rg)
{
	if (IsRendered)
	{
		mesh.LinkTechniques(rg);
	}
}

void PointLight::Submit(size_t channels) const noxnd
{
	if (IsRendered)
	{
		mesh.SetPos(cbData.pos);
		mesh.Submit(channels);
	}
}

void PointLight::Bind(Graphics& gfx, DirectX::FXMMATRIX view) const noexcept
{
	auto dataCopy = cbData;
	const auto pos = DirectX::XMLoadFloat3(&cbData.pos);
	DirectX::XMStoreFloat3(&dataCopy.pos, DirectX::XMVector3Transform(pos, view));
	cbuf.Update(gfx, dataCopy);
	cbuf.Bind(gfx);
}

PointLightCBuf PointLight::GetData() const noexcept
{
	return cbData;
}

void PointLight::Reset() noexcept
{
	cbData = home;
}

std::shared_ptr<Camera> PointLight::ShareCamera() const noexcept
{
	return pCamera;
}
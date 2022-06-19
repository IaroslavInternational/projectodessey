#include "Plate.h"
#include "Plane.h"
#include "BindableCommon.h"
#include "DynamicConstant.h"
#include "TechniqueProbe.h"
#include "Channels.h"
#include "ConstantBuffersEx.h"

Plate::Plate(Graphics&		   gfx,
			 DirectX::XMFLOAT2 size,
			 DirectX::XMFLOAT3 position,
			 DirectX::XMFLOAT3 orientation,
			 DirectX::XMFLOAT4 color)
	:
	size(size),
	position(position),
	orientation(orientation),
	gfx(gfx),
	pmc( { color } ),
	model(Plane::Make(size.x, size.y))
{
	CreatePlate();
}

void Plate::SetSize(DirectX::XMFLOAT2 size)
{
	this->size = size;
}

void Plate::SetPosition(DirectX::XMFLOAT3 position) noexcept
{
	this->position = position;
	this->position.y -= size.y / 2;
	this->position.z -= size.x / 2;
}

void Plate::SetRotation(DirectX::XMFLOAT3 orientation) noexcept
{
	this->orientation = orientation;
}

void Plate::UpdateSize(DirectX::XMFLOAT2 size)
{
	model = Plane::Make(size.x, size.y);
	CreatePlate();
}

DirectX::BoundingBox& Plate::CreateBBox()
{
	DirectX::BoundingBox box;
	box.Center = position;
	box.Extents = { size.x / 2, size.y / 2, 2.5 / 2 };

	return box;
}

DirectX::XMFLOAT2 Plate::GetSize() const noexcept
{
	return size;
}

DirectX::XMFLOAT2* Plate::GetSizePtr() noexcept
{
	return &size;
}

DirectX::XMMATRIX Plate::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(orientation.x, orientation.y, orientation.z) *
		   DirectX::XMMatrixTranslation(position.x, position.y, position.z);
}

void Plate::CreatePlate()
{
	using namespace Bind;

	const auto geometryTag = "$plane." + std::to_string(size.x) + "/" + std::to_string(size.y);
	pVertices = VertexBuffer::Resolve(gfx, geometryTag, model.vertices);
	pIndices = IndexBuffer::Resolve(gfx, geometryTag, model.indices);
	pTopology = Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	auto tcb = std::make_shared<TransformCbuf>(gfx);
	{
		//{
		//	Technique outline(Chan::main);
		//	{
		//		Step mask("outlineMask");
		//
		//		// TODO: better sub-layout generation tech for future consideration maybe
		//		mask.AddBindable(InputLayout::Resolve(gfx, model.vertices.GetLayout(), *VertexShader::Resolve(gfx, "Solid_VS.cso")));
		//
		//		mask.AddBindable(std::move(tcb));
		//
		//		// TODO: might need to specify rasterizer when doubled-sided models start being used
		//
		//		outline.AddStep(std::move(mask));
		//	}
		//	{
		//		Step draw("outlineDraw");
		//
		//		Dcb::RawLayout lay;
		//		lay.Add<Dcb::Float4>("color");
		//		auto buf = Dcb::Buffer(std::move(lay));
		//		buf["color"] = pmc.color;
		//		draw.AddBindable(std::make_shared<Bind::CachingPixelConstantBufferEx>(gfx, buf, 1u));
		//
		//		// TODO: better sub-layout generation tech for future consideration maybe
		//		draw.AddBindable(InputLayout::Resolve(gfx, model.vertices.GetLayout(), *VertexShader::Resolve(gfx, "Solid_VS.cso")));
		//
		//		draw.AddBindable(std::make_shared<TransformCbuf>(gfx));
		//
		//		// TODO: might need to specify rasterizer when doubled-sided models start being used
		//
		//		outline.AddStep(std::move(draw));
		//	}
		//	AddTechnique(std::move(outline));
		//}

		{
			Technique solid{ Chan::main };
			Step only("lambertian");

			auto pvs = VertexShader::Resolve(gfx, "Solid_VS.cso");
			only.AddBindable(InputLayout::Resolve(gfx, model.vertices.GetLayout(), *pvs));
			only.AddBindable(std::move(pvs));

			only.AddBindable(PixelShader::Resolve(gfx, "Solid_PS.cso"));

			struct PSColorConstant
			{
				DirectX::XMFLOAT3 color = { 0.12f,0.56f,1.0f };
				float padding;
			} colorConst;
			only.AddBindable(PixelConstantBuffer<PSColorConstant>::Resolve(gfx, colorConst, 1u));

			only.AddBindable(std::make_shared<TransformCbuf>(gfx));

			only.AddBindable(Rasterizer::Resolve(gfx, false));

			solid.AddStep(std::move(only));
			AddTechnique(std::move(solid));
		}
	}
}

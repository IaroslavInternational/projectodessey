#pragma once

#include "TechniqueProbe.h"
#include "ModelProbe.h"
#include "imgui/imgui.h"
#include "Technique.h"
#include "DynamicConstant.h"
#include "Model.h"
#include "Node.h"

#include <DirectXMath.h>
#include <string>
#include <unordered_map>

#include "EngineXM.h"

namespace dx = DirectX;

// Параметры мэша
class TP : public TechniqueProbe
{
public:
	void OnSetTechnique() override
	{
		using namespace std::string_literals;
		ImGui::TextColored( { 0.4f,1.0f,0.6f,1.0f },pTech->GetName().c_str() );
		bool active = pTech->IsActive();
		ImGui::Checkbox( ("Tech Active##"s + std::to_string( techIdx )).c_str(),&active );
		pTech->SetActiveState( active );
	}
	bool OnVisitBuffer( Dcb::Buffer& buf ) override
	{
		namespace dx = DirectX;
		float dirty = false;
		const auto dcheck = [&dirty]( bool changed ) {dirty = dirty || changed; };
		auto tag = [tagScratch = std::string{},tagString = "##" + std::to_string( bufIdx )]
		( const char* label ) mutable
		{
			tagScratch = label + tagString;
			return tagScratch.c_str();
		};

		if( auto v = buf["scale"]; v.Exists() )
		{
			dcheck( ImGui::SliderFloat( tag( "Размер" ),&v,1.0f,2.0f,"%.3f",3.5f ) );
		}
		if( auto v = buf["offset"]; v.Exists() )
		{
			dcheck( ImGui::SliderFloat( tag( "Отступ" ),&v,0.0f,1.0f,"%.3f",2.5f ) );
		}
		if( auto v = buf["materialColor"]; v.Exists() )
		{
			dcheck( ImGui::ColorPicker3( tag( "Цвет" ),reinterpret_cast<float*>(&static_cast<dx::XMFLOAT3&>(v)) ) );
		}
		if( auto v = buf["specularColor"]; v.Exists() )
		{
			dcheck( ImGui::ColorPicker3( tag( "Зерк., цвет" ),reinterpret_cast<float*>(&static_cast<dx::XMFLOAT3&>(v)) ) );
		}
		if( auto v = buf["specularGloss"]; v.Exists() )
		{
			dcheck( ImGui::SliderFloat( tag( "Глянец" ),&v,1.0f,100.0f,"%.1f",1.5f ) );
		}
		if( auto v = buf["specularWeight"]; v.Exists() )
		{
			dcheck( ImGui::SliderFloat( tag( "Зерк., вес" ),&v,0.0f,2.0f ) );
		}
		if( auto v = buf["useSpecularMap"]; v.Exists() )
		{
			dcheck( ImGui::Checkbox( tag( "Зерк., карта" ),&v ) );
		}
		if( auto v = buf["useNormalMap"]; v.Exists() )
		{
			dcheck( ImGui::Checkbox( tag( "Карта норм.," ),&v ) );
		}
		if( auto v = buf["normalMapWeight"]; v.Exists() )
		{
			dcheck( ImGui::SliderFloat( tag( "Вес карты норм." ),&v,0.0f,2.0f ) );
		}
		return dirty;
	}
};

// Параметры модели
class MP : ModelProbe
{
public:
	MP(std::string name) 
		: 
		name(std::move(name))
	{}

	void SpawnWindow(Model& model)
	{
		ImGui::Begin( name.c_str() );
		ImGui::Columns( 2,nullptr,true );
		model.Accept( *this );

		ImGui::NextColumn();
		if( pSelectedNode != nullptr )
		{
			bool dirty = false;
			const auto dcheck = [&dirty]( bool changed ) {dirty = dirty || changed; };
			auto& tf = ResolveTransform();
			ImGui::TextColored( { 0.4f,1.0f,0.6f,1.0f },"Позиция" );
			dcheck( ImGui::SliderFloat( "X",&tf.x,-60.f,60.f ) );
			dcheck( ImGui::SliderFloat( "Y",&tf.y,-60.f,60.f ) );
			dcheck( ImGui::SliderFloat( "Z",&tf.z,-60.f,60.f ) );
			ImGui::TextColored( { 0.4f,1.0f,0.6f,1.0f },"Ориентация" );
			dcheck( ImGui::SliderAngle( "X-вращение",&tf.xRot,-180.0f,180.0f ) );
			dcheck( ImGui::SliderAngle( "Y-вращение",&tf.yRot,-180.0f,180.0f ) );
			dcheck( ImGui::SliderAngle( "Z-вращение",&tf.zRot,-180.0f,180.0f ) );
			if( dirty )
			{
				pSelectedNode->SetAppliedTransform(
					dx::XMMatrixRotationX( tf.xRot ) *
					dx::XMMatrixRotationY( tf.yRot ) *
					dx::XMMatrixRotationZ( tf.zRot ) *
					dx::XMMatrixTranslation( tf.x,tf.y,tf.z )
				);
			}

			TP probe;
			pSelectedNode->Accept( probe );
		}
		ImGui::End();
	}

	void SpawnChildWindow(Model& model)
	{
		ImGui::BeginChild(name.c_str());
		ImGui::Columns(2, nullptr, true);
		model.Accept(*this);

		ImGui::NextColumn();
		if (pSelectedNode != nullptr)
		{
			bool dirty = false;
			const auto dcheck = [&dirty](bool changed) {dirty = dirty || changed; };
			auto& tf = ResolveTransform();
			ImGui::TextColored({ 0.4f,1.0f,0.6f,1.0f }, "Позиция");
			dcheck(ImGui::SliderFloat("X", &tf.x, -60.f, 60.f));
			dcheck(ImGui::SliderFloat("Y", &tf.y, -60.f, 60.f));
			dcheck(ImGui::SliderFloat("Z", &tf.z, -60.f, 60.f));
			ImGui::TextColored({ 0.4f,1.0f,0.6f,1.0f }, "Ориентация");
			dcheck(ImGui::SliderAngle("X-вращение", &tf.xRot, -180.0f, 180.0f));
			dcheck(ImGui::SliderAngle("Y-вращение", &tf.yRot, -180.0f, 180.0f));
			dcheck(ImGui::SliderAngle("Z-вращение", &tf.zRot, -180.0f, 180.0f));
			if (dirty)
			{
				pSelectedNode->SetAppliedTransform(
					dx::XMMatrixRotationX(tf.xRot) *
					dx::XMMatrixRotationY(tf.yRot) *
					dx::XMMatrixRotationZ(tf.zRot) *
					dx::XMMatrixTranslation(tf.x, tf.y, tf.z)
				);
			}

			TP probe;
			pSelectedNode->Accept(probe);
		}
		ImGui::EndChild();
	}
protected:
	bool PushNode(Node& node) override
	{
		// if there is no selected node, set selectedId to an impossible value
		const int selectedId = (pSelectedNode == nullptr) ? -1 : pSelectedNode->GetId();
		// build up flags for current node
		const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow
			| ((node.GetId() == selectedId) ? ImGuiTreeNodeFlags_Selected : 0)
			| (node.HasChildren() ? 0 : ImGuiTreeNodeFlags_Leaf);
		// render this node
		const auto expanded = ImGui::TreeNodeEx(
			(void*)(intptr_t)node.GetId(),
			node_flags,node.GetName().c_str()
		);
		// processing for selecting node
		if( ImGui::IsItemClicked() )
		{
			pSelectedNode = &node;
		}
		// signal if children should also be recursed
		return expanded;
	}

	void PopNode(Node& node) override
	{
		ImGui::TreePop();
	}
private:
	Node* pSelectedNode = nullptr;
	struct TransformParameters
	{
		float xRot = 0.0f;
		float yRot = 0.0f;
		float zRot = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};

	std::string name;
	std::unordered_map<int, TransformParameters> transformParams;
public:
	dx::XMFLOAT3 GetCurrentPosition()
	{
		dx::XMFLOAT3 pos = 
		{ 
			ResolveTransform().x, ResolveTransform().y, ResolveTransform().z 
		};

		return pos;
	}

	dx::XMFLOAT3 GetCurrentOrientation()
	{
		dx::XMFLOAT3 orient =
		{
			ResolveTransform().xRot, ResolveTransform().yRot, ResolveTransform().zRot
		};

		return orient;
	}
private:
	TransformParameters& ResolveTransform() noexcept
	{
		const auto id = pSelectedNode->GetId();
		auto i = transformParams.find( id );
		if( i == transformParams.end() )
		{
			return LoadTransform( id );
		}
		return i->second;
	}

	TransformParameters& LoadTransform( int id ) noexcept
	{
		const auto& applied = pSelectedNode->GetAppliedTransform();
		const auto angles = ExtractEulerAngles( applied );
		const auto translation = ExtractTranslation( applied );
		TransformParameters tp;
		tp.zRot = angles.z;
		tp.xRot = angles.x;
		tp.yRot = angles.y;
		tp.x = translation.x;
		tp.y = translation.y;
		tp.z = translation.z;
		return transformParams.insert( { id,{ tp } } ).first->second;
	}
};
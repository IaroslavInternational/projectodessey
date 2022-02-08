#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ModelException.h"
#include "Node.h"
#include "Mesh.h"
#include "Material.h"
#include "EngineXM.h"

namespace dx = DirectX;

Model::Model(std::string		name, 
			 const std::string& path,
			 Graphics&			gfx,
			 DirectX::XMFLOAT3  position,
			 DirectX::XMFLOAT3  orientation,
			 float				scale, 
			 bool				visibility)
	:
	VisibleObject(name, position, orientation, visibility)
{
	Assimp::Importer imp;
	auto pScene = imp.ReadFile(path.c_str(),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace |
		aiAnimBehaviour_REPEAT
	);

	if (pScene == nullptr)
	{
		throw ModelException(__LINE__, __FILE__, imp.GetErrorString());
	}

	// parse materials
	std::vector<Material> materials;
	materials.reserve( pScene->mNumMaterials );
	for (size_t i = 0; i < pScene->mNumMaterials; i++)
	{
		materials.emplace_back(gfx, *pScene->mMaterials[i], path);
	}

	for (size_t i = 0; i < pScene->mNumMeshes; i++)
	{
		const auto& mesh = *pScene->mMeshes[i];
		meshPtrs.push_back(std::make_unique<Mesh>(gfx, materials[mesh.mMaterialIndex], mesh, scale));
	}

	int nextId = 0;
	pRoot = ParseNode(nextId, *pScene->mRootNode, scale);
}

void Model::LinkTechniques(Rgph::RenderGraph& rg)
{
	if (visibility)
	{
		for (auto& pMesh : meshPtrs)
		{
			pMesh->LinkTechniques(rg);
		}
	}
}

void Model::Submit(size_t channels)
{
	if (visibility)
	{
		pRoot->Submit(channels, dx::XMMatrixIdentity());
	}
}

void Model::Accept(ModelProbe& probe)
{
	pRoot->Accept(probe);
}

void Model::SetRootTransform(DirectX::XMMATRIX tf) noexcept
{
	pRoot->SetAppliedTransform(tf);
}

void Model::Translate(DirectX::XMFLOAT3 delta)
{
	position.x += delta.x;
	position.y += delta.y;
	position.z += delta.z;

	SetRootTransform
	(
		DirectX::XMMatrixTranslation(
			position.x,
			position.y,
			position.z
		)
	);
}

void Model::Rotate(DirectX::XMFLOAT3 delta)
{
	orientation.x += delta.x;
	orientation.y += delta.y;
	orientation.z += delta.z;

	SetRootTransform
	(
		DirectX::XMMatrixRotationX(orientation.x) *
		DirectX::XMMatrixRotationY(orientation.y) *
		DirectX::XMMatrixRotationZ(orientation.z)
	);
}

std::unique_ptr<Node> Model::ParseNode(int& nextId, const aiNode& node, float scale) noexcept
{
	const auto transform = ScaleTranslation(dx::XMMatrixTranspose(dx::XMLoadFloat4x4(
		reinterpret_cast<const dx::XMFLOAT4X4*>(&node.mTransformation)
	)), scale);

	std::vector<Mesh*> curMeshPtrs;
	curMeshPtrs.reserve(node.mNumMeshes);
	
	for (size_t i = 0; i < node.mNumMeshes; i++)
	{
		const auto meshIdx = node.mMeshes[i];
		curMeshPtrs.push_back(meshPtrs.at(meshIdx).get());
	}

	auto pNode = std::make_unique<Node>(nextId++, node.mName.C_Str(), std::move(curMeshPtrs), transform);

	for (size_t i = 0; i < node.mNumChildren; i++)
	{
		pNode->AddChild(ParseNode(nextId, *node.mChildren[i], scale));
	}

	return pNode;
}
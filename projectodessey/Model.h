#pragma once

#include "VisibleObject.h"

#include <memory>

class Node;
class Mesh;
class ModelWindow;

struct aiMesh;
struct aiMaterial;
struct aiNode;
struct aiAnimation;

namespace Rgph
{
	class RenderGraph;
}

// ����� ��� �������� ����������� 3-D �������. 
class Model : public VisibleObject
{
public:
	friend class GUISystem;
public:
	// ����������� ������ ������� ��������� ����:
	// name - ��� ������
	// path - ���� � �������
	// gfx - ����� ������������ ����
	// position - ������� ������
	// orientation - ���������� ������
	// visibility - ��������� ������
	Model(std::string		 name, 
		  const std::string& path, 
		  Graphics&			 gfx, 
		  DirectX::XMFLOAT3	 position =    { 0.0f, 0.0f, 0.0f }, 
		  DirectX::XMFLOAT3	 orientation = { 0.0f, 0.0f, 0.0f },
		  float				 scale =	   1.0f, 
		  bool				 visibility =  true);
public:
	void LinkTechniques(Rgph::RenderGraph& rg);		      // ����� ��� ������������� ������ � Render Graph			
	void Submit(size_t channels);					      // ����� ��� ������������� ������ � ������ ���������
	void Accept(class ModelProbe& probe);			      // ������� ��������� ������������ ���������� ��� ������
	void SetRootTransform(DirectX::XMMATRIX tf) noexcept; // ���������� ��������� � ������������ � ������� �������	
private:
	std::unique_ptr<Node> ParseNode(int& nextId, const aiNode& node, float scale) noexcept; // ������� ������� (��������� ������) ������
private:
	std::unique_ptr<Node> pRoot;				 // ��������� �� ��������� ������						
	std::vector<std::unique_ptr<Mesh>> meshPtrs; // ��������� �� ����	
};
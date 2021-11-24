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

// Класс для описания функционала 3-D моделей. 
class Model : public VisibleObject
{
public:
	friend class GUISystem;
public:
	// Конструктор модели требует следующие поля:
	// name - имя модели
	// path - путь к объекту
	// gfx - адрес графического ядра
	// position - позиция модели
	// orientation - ориентация модели
	// visibility - видимость модели
	Model(std::string		 name, 
		  const std::string& path, 
		  Graphics&			 gfx, 
		  DirectX::XMFLOAT3	 position =    { 0.0f, 0.0f, 0.0f }, 
		  DirectX::XMFLOAT3	 orientation = { 0.0f, 0.0f, 0.0f },
		  float				 scale =	   1.0f, 
		  bool				 visibility =  true);
public:
	void LinkTechniques(Rgph::RenderGraph& rg);		      // Метод для присоединения модели к Render Graph			
	void Submit(size_t channels);					      // Метод для присоединения модели к каналу отрисовки
	void Accept(class ModelProbe& probe);			      // Создать экземпляр расширенного интерфейса для модели
	void SetRootTransform(DirectX::XMMATRIX tf) noexcept; // Установить положение в пространстве с помощью матрицы	
private:
	std::unique_ptr<Node> ParseNode(int& nextId, const aiNode& node, float scale) noexcept; // Парсинг записей (составных частей) модели
private:
	std::unique_ptr<Node> pRoot;				 // Указатель на структуру модели						
	std::vector<std::unique_ptr<Mesh>> meshPtrs; // Указатели на меши	
};
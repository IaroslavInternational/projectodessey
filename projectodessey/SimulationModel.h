#pragma once

#include "Robot.h"
#include "DataParser.h"
#include <vector>
#include "SolidSphere.h"

enum class SimFlag
{
	Default,
	Collision
};

class SimulationModel
{
public:
	friend class GUISystem;
public:
	SimulationModel(const std::string& file_pos, const std::string& file_orient, const std::string& file_t, Robot& r, Graphics& gfx, Rgph::RenderGraph& rg);
public:
	void LinkTechniques();
	void Draw(size_t channels);
public:
	void Start() noexcept;
	void Stop(SimFlag flag) noexcept;
	void Refresh();
	void Reload();
	void Simulate(float dt);
private:
	void Init();
private:
	std::vector<std::unique_ptr<SolidSphere>> points;
	std::unique_ptr<DataParser> data;
	Robot&     robot;
private:
	const std::string file_pos;
	const std::string file_orient;
	const std::string file_t;
private:
	Graphics& gfx;
	Rgph::RenderGraph& rg;
	bool   OnSim = false;
	size_t iteration = 0;
	float  time_counter = 0.0f;
	float  speed = 1.0f;
	bool CollisionDetected = false;
private:
	struct
	{
		float  max;
		float  min;
		float  step;
	} timeArgs;
	struct
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 orientation;
		DirectX::XMFLOAT3 velocity;
		DirectX::XMFLOAT3 axis_velocity;
	} initialState;
	struct
	{
		DirectX::XMFLOAT3 linear = { 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT3 angle = { 0.0f, 0.0f, 0.0f };;
	} tau;
};
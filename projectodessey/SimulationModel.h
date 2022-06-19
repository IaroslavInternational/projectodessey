#pragma once

#include "Robot.h"
#include "DataParser.h"
#include <vector>
#include "SolidSphere.h"

class SimulationModel
{
public:
	friend class GUISystem;
public:
	SimulationModel(const std::string& file_pos, const std::string& file_orient, const std::string& file_t, Robot& r, Graphics& gfx);
public:
	void LinkTechniques(Rgph::RenderGraph& rg);
	void Draw(size_t channels);
public:
	void Start() noexcept;
	void Stop() noexcept;
	void Refresh();
	void Simulate(float dt);
private:
	std::vector<std::unique_ptr<SolidSphere>> points;
	DataParser data;
	Robot&     robot;
private:
	bool   OnSim = false;
	size_t iteration = 0;
	float  time_counter = 0.0f;
	float  speed = 1.0f;
private:
	struct
	{
		float  max = 50.0f;
		float  min = 0.0f;
		float  step = 0.01f;
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
		DirectX::XMFLOAT3 linear;
		DirectX::XMFLOAT3 angle;
	} tau;
};
#pragma once

#include "Robot.h"
#include "DataParser.h"

class SimulationModel
{
public:
	friend class GUISystem;
public:
	SimulationModel(const std::string& file_pos, const std::string& file_orient, const std::string& file_t, Robot& r);
public:
	void Start() noexcept;
	void Stop() noexcept;
	void Refresh();
	void Simulate(float dt);
private:
	DataParser data;
	Robot&     robot;
private:
	bool   OnSim = false;
	size_t iteration = 0;
	float  time_counter = 0.0f;
	float  speed = 1.0f;
};


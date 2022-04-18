#include "SimulationModel.h"

SimulationModel::SimulationModel(const std::string& file_pos, const std::string& file_orient, const std::string& file_t, Robot& r)
	:
	data(file_pos, file_orient, file_t),
	robot(r)
{
}

void SimulationModel::Start() noexcept
{
	OnSim = true;
}

void SimulationModel::Stop() noexcept
{
	OnSim = false;
}

void SimulationModel::Refresh()
{
	iteration = 0;

	robot.SetPosition({ 0.0f, 0.0f , 0.0f });
	robot.SetOrientation({ 0.0f, 0.0f , 0.0f });

	OnSim = false;
}

void SimulationModel::Simulate(float dt)
{
	if (!OnSim)
	{
		return;
	}

	if (data.GetTime(iteration) >= time_counter)
	{
		if (!(iteration < data.GetIterations()))
		{
			return;
		}

		robot.SetPosition(data.GetPosition(iteration));
		robot.SetOrientation(data.GetOrientation(iteration));

		iteration++;
	}
	else
	{
		time_counter += dt * speed;
	}
}

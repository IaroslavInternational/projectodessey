#include "SimulationModel.h"

SimulationModel::SimulationModel(const std::string& file_pos, const std::string& file_orient, const std::string& file_t, Robot& r, Graphics& gfx, Rgph::RenderGraph& rg)
	:
	gfx(gfx),
	rg(rg),
	file_pos(file_pos),
	file_orient(file_orient),
	file_t(file_t),
	data(std::make_unique<DataParser>(file_pos, file_orient, file_t)),
	robot(r)
{
	Init();
}

void SimulationModel::LinkTechniques()
{
	for (auto& p : points)
	{
		p->LinkTechniques(rg);
	}
}

void SimulationModel::Draw(size_t channels)
{
	for (auto& p : points)
	{
		p->Submit(channels);
	}
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
	time_counter = 0.0f;
	speed = 1.0f;

	robot.SetPosition(initialState.position);
	robot.SetOrientation(initialState.orientation);

	std::string cmd = "matlab -noFigureWindows -nosplash -automation -sd \"C:\\Users\\Yaros\\source\\projectodessey\\projectodessey\\model\\\" -batch main";
	system(cmd.c_str());

	OnSim = false;
}

void SimulationModel::Reload()
{
	data.release();
	data = std::make_unique<DataParser>(file_pos, file_orient, file_t);

	Init();
}

void SimulationModel::Simulate(float dt)
{
	if (!OnSim)
	{
		return;
	}

	time_counter += dt * speed;
	while (time_counter >= data->GetTime(iteration))
	{
		if (!(iteration < data->GetIterations()))
		{
			OnSim = false;
			return;
		}

		robot.SetPosition(data->GetPosition(iteration));
		robot.SetOrientation(data->GetOrientation(iteration));

		iteration++;
	}
}

void SimulationModel::Init()
{
	initialState.position = robot.GetPosition();
	initialState.orientation = robot.GetOrientation();
	initialState.velocity = { 0.0f, 0.0f, 0.0f };
	initialState.axis_velocity = { 0.0f, 0.0f, 0.0f };

	timeArgs.min = data->GetTime(0);
	timeArgs.max = data->GetTime(data->GetIterations() - 1);
	timeArgs.step = data->GetTime(1) - data->GetTime(0);

	points.clear();

	for (size_t i = 0; i < data->GetIterations(); i += 10)
	{
		points.push_back(std::make_unique<SolidSphere>(gfx, 0.1f));
		points.back()->SetPos(data->GetPosition(i));
	}

	LinkTechniques();
}
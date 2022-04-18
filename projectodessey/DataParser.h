#pragma once

#include <DirectXMath.h>
#include <vector>
#include <string>

class DataParser
{
public:
	DataParser(const std::string& file_pos, const std::string& file_orient, const std::string& file_t);
public:
	DirectX::XMFLOAT3 GetPosition(size_t index) const noexcept;
	DirectX::XMFLOAT3 GetOrientation(size_t index) const noexcept;
	float			  GetTime(size_t index) const noexcept;
public:
	size_t GetIterations() const noexcept;
private:
	std::vector<std::string> GetLines(const std::string& filename) const;
private:
	std::vector<DirectX::XMFLOAT3> positions;
	std::vector<DirectX::XMFLOAT3> orientations;
	std::vector<float> times;
private:
	size_t sim_iterations;
};

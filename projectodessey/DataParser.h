#pragma once

#include <DirectXMath.h>
#include <vector>
#include <string>

class DataParser
{
public:
	DataParser(const std::string& file_pos, const std::string& file_orient, const std::string& file_t);
private:
	std::vector<std::string> GetLines(const std::string& filename) const;
private:
	std::vector<DirectX::XMFLOAT3> positions;
	std::vector<DirectX::XMFLOAT3> orientations;
	std::vector<float> times;
};

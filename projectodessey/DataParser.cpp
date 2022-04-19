#include "DataParser.h"

#include <future>
#include <fstream> 
#include <regex>

DataParser::DataParser(const std::string& file_pos, const std::string& file_orient, const std::string& file_t)
{ 
    auto d0 = std::async(&DataParser::GetLines, this, file_pos);
    auto d1 = std::async(&DataParser::GetLines, this, file_orient);
    auto d2 = std::async(&DataParser::GetLines, this, file_t);

    std::vector<std::string> data_p{ d0.get() };
    std::vector<std::string> data_o{ d1.get() };
    std::vector<std::string> data_t{ d2.get() };

    static const std::regex splitter{ "," };

    for (auto& line : data_p)
    {
        std::vector<std::string> vals
        {
                std::sregex_token_iterator(line.begin(), line.end(), splitter, -1),
                std::sregex_token_iterator()
        };

        positions.push_back({ std::stof(vals[0]), std::stof(vals[1]), std::stof(vals[2]) });
    }

    for (auto& line : data_o)
    {
        std::vector<std::string> vals
        {
                std::sregex_token_iterator(line.begin(), line.end(), splitter, -1),
                std::sregex_token_iterator()
        };

        orientations.push_back({ std::stof(vals[0]), std::stof(vals[1]), std::stof(vals[2]) });
    }

    for (auto& line : data_t)
    {
        times.push_back(std::stof(line));
    }

    sim_iterations = times.size();

    positions.shrink_to_fit();
    orientations.shrink_to_fit();
    times.shrink_to_fit();
}

DirectX::XMFLOAT3 DataParser::GetPosition(size_t index) const noexcept
{
    return positions[index];
}

DirectX::XMFLOAT3 DataParser::GetOrientation(size_t index) const noexcept
{
    return orientations[index];
}

float DataParser::GetTime(size_t index) const noexcept
{
    return times[index];
}

size_t DataParser::GetIterations() const noexcept
{
    return sim_iterations;
}

std::vector<std::string> DataParser::GetLines(const std::string& filename) const
{
    std::string s;
    std::vector<std::string> lines;

    std::ifstream file(filename);

    while (std::getline(file, s))
    {
        lines.push_back(s);
    }

    file.close();

    return lines;
}

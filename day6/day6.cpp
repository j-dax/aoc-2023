#include <fstream>
#include <iostream>
#include <ranges>
#include <regex>
#include <unordered_map>

namespace day6
{
void part1(const std::string& filepath)
{
    std::ifstream file(filepath);
    for (const std::string& line : std::ranges::istream_view<std::string>(file))
    {
    }
    std::cout << "Total: " << 0 << std::endl;
}

void part2(const std::string& filepath)
{
    std::ifstream file(filepath);
    for (const std::string& line : std::ranges::istream_view<std::string>(file))
    {
    }
    std::cout << "Total: " << 0 << std::endl;
}
}

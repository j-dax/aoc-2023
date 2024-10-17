#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

namespace day3
{
struct SymbolInfo
{
    char symbol;
    int row;
    int col;
    std::vector<int> surrounding_numbers;
};

std::vector<SymbolInfo> get_symbols(const std::string& path)
{
    std::ifstream file(path);
    auto grid_view =
        std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>())
        | std::views::split('\n');
    auto grid = std::vector(grid_view.begin(), grid_view.end());
    std::vector<SymbolInfo> result;

    auto is_symbol = [](char c)
    {
        return c != '.' && !std::isdigit(c);
    };

    for (int row = 0; row < grid.size(); ++row)
    {
        for (int col = 0; col < grid[row].size(); ++col)
        {
            if (is_symbol(grid[row][col]))
            {
                SymbolInfo info { grid[row][col], row, col, {} };

                // Check surrounding cells
                for (int dr = -1; dr <= 1; ++dr)
                {
                    for (int dc = -1; dc <= 1; ++dc)
                    {
                        int r = row + dr;
                        int c = col + dc;

                        if (r >= 0 && r < grid.size() && c >= 0 && c < grid[r].size()
                            && std::isdigit(grid[r][c]))
                        {
                            // Find the start of the number
                            while (c > 0 && std::isdigit(grid[r][c - 1]))
                                --c;

                            // Extract the full number
                            std::string number;
                            while (c < grid[r].size() && std::isdigit(grid[r][c]))
                            {
                                number += grid[r][c];
                                ++c;
                            }

                            info.surrounding_numbers.push_back(std::stoi(number));
                        }
                    }
                }

                // Remove duplicates
                std::ranges::sort(info.surrounding_numbers);
                auto [first, last] = std::ranges::unique(info.surrounding_numbers);
                info.surrounding_numbers.erase(first, last);

                result.push_back(std::move(info));
            }
        }
    }
    return result;
}

void part1(const std::string& filename)
{
    size_t sum = 0;
    auto result = get_symbols(filename);
    for (auto r : result)
    {
        sum += std::accumulate(r.surrounding_numbers.begin(), r.surrounding_numbers.end(), 0);
    }
    std::cout << "Total: " << sum << std::endl;
}

void part2(const std::string& filename)
{
    size_t sum = 0;
    for (auto r : get_symbols(filename) | std::views::filter([](auto& r1) { }))
    {
        if (r.symbol == '*' && r.surrounding_numbers.size() == 2)
        {
            sum += r.surrounding_numbers[0] * r.surrounding_numbers[1];
        }
    }
    std::cout << "Total: " << sum << std::endl;
}
}

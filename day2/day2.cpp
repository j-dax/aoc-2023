#include <fstream>
#include <iostream>
#include <ranges>
#include <vector>

namespace day2
{
// Cube Conundrum
// Given some default inputs, how many games were possible?
struct RGBPull
{
    int r;
    int g;
    int b;
};

struct RGBGame
{
    int id;
    std::vector<RGBPull> pulls;
};

// given " green 2", return {.r=0, .g=2, .b=0}
RGBPull parsePull(auto&& pull)
{
    auto trimmed = std::views::drop_while(pull, isspace);
    auto str = std::string(trimmed.begin(), trimmed.end());
    int space_index = str.find(' ');
    char ch = str[space_index + 1];
    int n = std::atoi(str.substr(0, space_index).data());

    auto member = &RGBPull::r;
    switch (ch)
    {
        case 'r':
            break;
        case 'g':
            member = &RGBPull::g;
            break;
        case 'b':
            member = &RGBPull::b;
            break;
        default:
            std::cerr << "unhandled: \"" << str << "\"" << std::endl;
    }

    RGBPull out {};
    out.*member = n;
    return out;
}

// Parses a line, extracting an RGBGame
RGBGame scanGame(const std::string& line)
{
    // Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
    auto id = std::atoi(line.substr(5, line.find_first_of(':')).data());
    auto result = line.substr(line.find_first_of(':') + 2) | std::views::split(';')
        | std::views::transform([](auto&& games) { return games | std::views::split(','); });
    std::vector<RGBPull> pulls;
    for (auto game : result)
    {
        for (auto pull : game)
        {
            pulls.push_back(parsePull(pull));
        }
    }
    return { id, pulls };
}

void part1(const std::string& filename)
{
    RGBPull given { 12, 13, 14 };
    unsigned long long sum = 0;

    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
        RGBGame game = scanGame(line);
        RGBPull max {};
        for (auto pull : game.pulls)
        {
            max.r = std::max(max.r, pull.r);
            max.g = std::max(max.g, pull.g);
            max.b = std::max(max.b, pull.b);
        }

        if (max.r <= given.r && max.g <= given.g && max.b <= given.b)
        {
            sum += game.id;
        }
    }
    file.close();

    std::cout << "Sum of ids: " << sum << std::endl;
}
void part2(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open " << filename << std::endl;
        return;
    }

    unsigned long long sum = 0;
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
        RGBGame game = scanGame(line);
        RGBPull max {};
        for (auto pull : game.pulls)
        {
            max.r = std::max(max.r, pull.r);
            max.g = std::max(max.g, pull.g);
            max.b = std::max(max.b, pull.b);
        }

        sum += max.r * max.g * max.b;
    }
    file.close();

    std::cout << "Total: " << sum << std::endl;
}
} // namespace day2

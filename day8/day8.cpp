#include <climits>
#include <fstream>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <vector>

namespace day8
{

struct Input
{
    std::string directions;
    std::unordered_map<std::string, std::pair<std::string, std::string>> graph;
};

Input get_input(const std::string& filename)
{
    std::ifstream file(filename);
    std::string directions;
    file >> directions;

    std::unordered_map<std::string, std::pair<std::string, std::string>> graph;
    std::string src, eq, left, right;
    while (file >> src >> eq >> left >> right)
    {
        // assume keys are size 3: AAA = (BBB, CCC)
        graph[src] = { left.substr(1, 3), right.substr(0, 3) };
    }

    return { directions, graph };
}

template <typename T>
concept Printable = requires(std::ostream& os, const T& t) {
    { os << t } -> std::same_as<std::ostream&>;
};

template <Printable T>
std::ostream& operator<<(std::ostream& os, std::vector<T> v)
{
    os << "[ ";
    for (auto vi : v)
    {
        os << vi << " ";
    }
    os << "]";
    return os;
}

void part1(const std::string& filename)
{
    std::string start = "AAA", end = "ZZZ";
    Input input = get_input(filename);

    size_t step = 0;
    while (start.compare(end) != 0)
    {
        char dir = input.directions[step++ % input.directions.size()];
        start = (dir == 'L') ? input.graph[start].first : input.graph[start].second;
    }
    std::cout << "Total: " << step << std::endl;
}

void part2(const std::string& filename)
{
    Input input = get_input(filename);

    std::unordered_map<std::string, size_t> steps_to_end;
    // initializer
    for (auto& [first, second] : input.graph)
    {
        if (first.ends_with('Z'))
        {
            steps_to_end[first] = 0;
        }
        else
        {
            steps_to_end[first] = ULLONG_MAX - 1;
        }
    }

    // reduce our problem surface to number of steps from current location to end
    for (size_t run = 1; run < steps_to_end.size(); run++)
    {
        for (auto& [first, second] : input.graph)
        {
            steps_to_end[first] = std::min(steps_to_end[first],
                std::min(steps_to_end[second.first] + 1, steps_to_end[second.second] + 1));
        }
    }

    size_t lcm = 1;
    for (auto& [first, second] : input.graph)
    {
        if (first.ends_with('A'))
        {
            lcm = std::lcm(steps_to_end[first], lcm);
        }
    }
    size_t minimum_moves = std::lcm(input.directions.size(), lcm);
    std::cout << "Total " << minimum_moves << std::endl;
}
};

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

namespace day9
{
std::vector<std::vector<int>> read_lines(const std::string& filename)
{
    std::vector<std::vector<int>> nums;
    std::ifstream file(filename);
    auto nonempty = [](auto&& s)
    {
        return !s.empty();
    };
    for (std::string line; std::getline(file, line);)
    {
        auto view = line | std::views::split(' ') | std::views::filter(nonempty)
            | std::views::transform(
                [](auto&& n_view)
                {
                    std::string n_str(n_view.begin(), n_view.end());
                    return std::stoi(n_str);
                });
        nums.push_back(std::vector<int>(view.begin(), view.end()));
    }
    return nums;
}
std::vector<std::vector<int>> regression(std::vector<int> first_history)
{
    std::vector<std::vector<int>> vecs { first_history };

    bool all_zero = std::ranges::all_of(vecs.back(), [](int n) { return n == 0; });
    while (!all_zero)
    {
        std::vector<int> next_vec;
        for (size_t i = 1; i < vecs.back().size(); i++)
        {
            next_vec.push_back(vecs.back()[i] - vecs.back()[i - 1]);
        }
        vecs.push_back(next_vec);
        all_zero = std::ranges::all_of(vecs.back(), [](int n) { return n == 0; });
    }

    return vecs;
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
    size_t sum = 0;
    for (auto history : read_lines(filename))
    {
        auto prev = 0;
        auto reg = regression(history);
        // add a single digit to the end of each regression, sum them
        for (auto it = std::next(reg.rbegin()); it != reg.rend(); it++)
        {
            prev = it->back();
            sum += prev;
        }
    }
    std::cout << "Total: " << sum << std::endl;
}
void part2(const std::string& filename)
{
    long long sum = 0;
    for (auto history : read_lines(filename))
    {
        auto prev = 0;
        auto reg = regression(history);
        // add a single digit to the end of each regression, sum them
        for (auto it = std::next(reg.rbegin()); it != reg.rend(); it++)
        {
            prev = it->front() - prev;
        }
        sum += prev;
    }
    std::cout << "Total: " << sum << std::endl;
}
};

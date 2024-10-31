#include <algorithm>
#include <cassert>
#include <charconv>
#include <climits>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#undef NDEBUG

namespace day6
{
struct Race
{
    int64_t time;
    int64_t distance;

    // return distance traveled within race parameters
    bool is_winning_race(size_t time_remaining)
    {
        return (time - time_remaining) * time_remaining > distance;
    }

    // returns the number of valid strategies
    uint64_t find_valid_strategies()
    {
        // delay (P), distance (d), velocity (v), total_time (T)
        // remaining_time (t)
        // v = T - t
        // d = v * t = (T - t)(T - (T - t)) = (T-t)(t) = Tt - t**2
        // Find a critical point ( possible local maxima )
        // d/dt = T - 2t = 0
        // maxmimal delta should be at time / 2
        // or highest total distance traveled
        // if there is no winning race, throw an error
        assert(is_winning_race(time / 2));

        uint64_t min = time >> 1;
        uint64_t delta = time >> 2;
        while (delta > 0)
        {
            while (is_winning_race(min - delta))
            {
                min -= delta;
            }
            delta >>= 1;
        }

        uint64_t max = time >> 1;
        delta = time >> 2;
        while (delta > 0)
        {
            while (is_winning_race(max + delta))
            {
                max += delta;
            }
            delta >>= 1;
        }

        return max - min + 1;
    }
};

std::vector<std::string> read_file(const std::string& filepath)
{
    std::ifstream file(filepath);
    std::vector<std::string> lines;

    for (std::string line; std::getline(file, line);)
    {
        lines.push_back(line);
    }

    return lines;
}

// Cut the header from the beginning of the string_view and extract the following numbers
// into a vector
std::vector<int64_t> line_to_vec(std::string_view str, size_t header_len)
{
    std::vector<int64_t> nums;
    auto num_start = str.find_first_not_of(' ', header_len);
    while (num_start != std::string_view::npos)
    {
        auto num_end = str.find(' ', num_start);
        int64_t value;
        auto [ptr, ec] = std::from_chars(str.data() + num_start,
            str.data() + std::min(num_end, str.size()), value);
        if (ec == std::errc())
        {
            nums.push_back(value);
        }
        num_start = str.find_first_not_of(' ', num_end);
    }
    return nums;
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

void part1(const std::string& filepath)
{
    auto lines = read_file(filepath);

    constexpr size_t time_header_len = std::string_view("Time: ").size();
    constexpr size_t distance_header_len = std::string_view("Distance: ").size();

    std::vector<int64_t> times = line_to_vec(lines[0], time_header_len);
    std::vector<int64_t> distances = line_to_vec(lines[1], distance_header_len);

    uint64_t product = 1;
    for (int i = 0; i < times.size(); i++)
    {
        Race r { .time = times[i], .distance = distances[i] };
        product *= r.find_valid_strategies();
    }
    std::cout << "Total: " << product << std::endl;
}

// Join each number in the vector as if it were a single number, read left to right
// [1 2 3] -> 123
int64_t join(std::vector<int64_t> nums)
{
    int64_t accumulator = nums.back();
    nums.pop_back();
    while (!nums.empty())
    {
        int pow = std::ceil(std::log10(accumulator));
        auto back = nums.back();
        nums.pop_back();
        accumulator += std::pow(10, pow) * back;
    }
    return accumulator;
}

void part2(const std::string& filepath)
{
    auto lines = read_file(filepath);

    constexpr size_t time_header_len = std::string_view("Time: ").size();
    constexpr size_t distance_header_len = std::string_view("Distance: ").size();

    int64_t time = join(line_to_vec(lines[0], time_header_len));
    int64_t distance = join(line_to_vec(lines[1], distance_header_len));
    Race r { .time = time, .distance = distance };

    std::cout << "Total: " << r.find_valid_strategies() << std::endl;
}
}

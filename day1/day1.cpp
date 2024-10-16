#include <fstream>
#include <iostream>
#include <ranges>
#include <regex>
#include <unordered_map>

namespace day1
{
// Trebuchet!?
// On each line, the calibration value can be found by combining the first digit and the last digit
// (in that order) to form a single two-digit number.
void part1(const std::string& filepath)
{
    std::ifstream file(filepath);
    std::string nums = "0123456789";

    unsigned long long sum = 0;
    for (const std::string& line : std::ranges::istream_view<std::string>(file))
    {
        if (!line.empty())
        {
            auto last_index = line.find_last_of(nums);
            char last = line[last_index] - '0';
            auto first_index = line.find_first_of(nums);
            char first = line[first_index] - '0';
            auto n = 10 * first + last;
            sum += n;
        }
    }
    std::cout << "Total: " << sum << std::endl;
}

// Trebuchet!?
// On each line, the calibration value can be found by combining the first digit and the last digit
// (in that order) to form a single two-digit number.
void part2(const std::string& filepath)
{
    std::ifstream file(filepath);
    std::string nums = "0123456789";

    unsigned long long sum = 0;
    for (std::string line : std::ranges::istream_view<std::string>(file))
    {
        // some digits are spelled out:
        // one two three four five six seven eight nine
        // Replace the above with their match below.
        // This ensures each digit that has overlap will be recognized: sevenineight
        // o1e t2o t3e 4 5e 6 7n e8t n9e
        std::unordered_map<std::string, std::string> m {
            { "one", "o1e" },
            { "two", "t2o" },
            { "three", "t3e" },
            { "four", "4" },
            { "five", "5e" },
            { "six", "6" },
            { "seven", "7n" },
            { "eight", "e8t" },
            { "nine", "n9e" },
        };
        for (const auto& [key, val] : m)
        {
            std::regex pattern(key);
            line = std::regex_replace(line, pattern, val);
        }
        if (!line.empty())
        {
            auto last_index = line.find_last_of(nums);
            char last = line[last_index] - '0';
            auto first_index = line.find_first_of(nums);
            char first = line[first_index] - '0';
            auto n = 10 * first + last;
            sum += n;
        }
    }
    std::cout << "Total: " << sum << std::endl;
}
}

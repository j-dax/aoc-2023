#include <iostream>
#include <fstream>
#include <ranges>
#include <string>

namespace day1 {
// Trebuchet!?
// On each line, the calibration value can be found by combining the first digit and the last digit (in that order) to form a single two-digit number.
void part1(const std::string& filepath) {
    std::ifstream file(filepath);
    std::string nums = "0123456789";

    unsigned long long sum = 0;
    for (const std::string& line : std::ranges::istream_view<std::string>(file)) {
        if (!line.empty()) {
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

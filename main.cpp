#include "day1/day1.cpp"
#include "day10/day10.cpp"
#include "day2/day2.cpp"
#include "day3/day3.cpp"
#include "day4/day4.cpp"
#include "day5/day5.cpp"
#include "day6/day6.cpp"
#include "day7/day7.cpp"
#include "day8/day8.cpp"
#include "day9/day9.cpp"

#include <iostream>
#include <sstream>
#include <string>

void temp(const std::string& f) { }

std::ifstream read_file(const std::string& filename) { return std::ifstream { filename }; }

void print_help() { std::cerr << "Usage: ./aoc <day> <part> <filename>" << std::endl; }

// Driver for aoc
// Expects ./aoc <day> <part> <filename>
// day and part must be numbers
// filename should contain the input data
// if filename is missing, the day*::part* function will be called with an empty string.
int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        print_help();
        return -1;
    }

    int day {};
    std::stringstream convert { argv[1] };
    if (!(convert >> day) || day <= 0)
    {
        std::cerr << "Failed to read day number" << std::endl;
        print_help();
        return -2;
    }

    int part {};
    convert = std::stringstream { argv[2] };
    if (!(convert >> part) || part <= 0)
    {
        std::cerr << "Failed to read part number" << std::endl;
        print_help();
        return -3;
    }

    std::string filename = "";
    if (argc > 2)
    {
        filename = argv[3];
    }

    void (*dayparts[][2])(const std::string&) = {
        { day1::part1, day1::part2 },
        { day2::part1, day2::part2 },
        { day3::part1, day3::part2 },
        { day4::part1, day4::part2 },
        { day5::part1, day5::part2 },
        { day6::part1, day6::part2 },
        { day7::part1, day7::part2 },
        { day8::part1, day8::part2 },
        { day9::part1, day9::part2 },
        { day10::part1, day10::part2 },
    };
    dayparts[day - 1][part - 1](filename);
}

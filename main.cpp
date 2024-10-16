#include "day1/part1.cpp"
#include "day1/part2.cpp"
#include "day2/day2.cpp"

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
    };
    dayparts[day - 1][part - 1](filename);
}

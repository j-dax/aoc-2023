#include "day1/part1.cpp"
#include "day2/part1.cpp"

#include <iostream>
#include <string>
#include <sstream>

void temp(const std::string& s) {}

std::string read_file(const std::string& filename) {
    return "";
}

void print_help() {
    std::cerr << "Usage: ./aoc <day> <part> <filename>" << std::endl;
}

// Driver for aoc
// Expects ./aoc <day> <part> <filename>
// day and part must be numbers
// filename should contain the input data
// if filename is missing, the day*::part* function will be called with an empty string.
int main(int argc, char* argv[]) {
    if (argc < 3) {
        print_help();
        return -1;
    }

    int day{};
    std::stringstream convert{argv[1]};
    if (!(convert >> day) || day <= 0) {
        std::cerr << "Failed to read day number" << std::endl;
        print_help();
        return -2;
    }

    int part{};
    convert = std::stringstream{argv[2]};
    if (!(convert >> part) || part <= 0) {
        std::cerr << "Failed to read part number" << std::endl;
        print_help();
        return -3;
    }

    std::string filename = "";
    std::string contents = "";

    if (argc > 3) {
        filename = argv[3];
        contents = read_file(filename);
    }

    void (*dayparts[][2])(const std::string&) = {
        {day1::part1, temp},
        {day2::part1, temp},
    };
    dayparts[day-1][part-1](contents);
}

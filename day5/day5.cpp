#include <algorithm>
#include <climits>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

using namespace std::literals;

namespace day5
{
struct Triple
{
    uint64_t destination;
    uint64_t source;
    uint64_t range;
};

struct FileReader
{
    std::string line;

    friend std::istream& operator>>(std::istream& is, FileReader& fr)
    {
        return std::getline(is, fr.line);
    }
};

std::ostream& operator<<(std::ostream& os, Triple t)
{
    os << "[ " << t.destination << " " << t.source << " " << t.range << " ]";
    return os;
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

struct SeedRange
{
    uint64_t source;
    uint64_t range;

    SeedRange intersect(uint64_t base, uint64_t length)
    {
        auto overlap_start = std::max(base, source);
        auto overlap_end = std::min(base + length, source + range);
        if (overlap_start <= overlap_end)
        {
            return { .source = overlap_start, .range = overlap_end - overlap_start };
        }
        return { .range = 0 };
    }
};

std::ostream& operator<<(std::ostream& os, SeedRange r)
{
    os << "[ " << r.source << " " << r.range << " ]";
    return os;
}

struct PromptInput
{
    std::vector<uint64_t> seeds;
    std::vector<std::vector<Triple>> conversions;

    std::vector<SeedRange> lookup(std::vector<Triple> convert, SeedRange r)
    {
        std::vector<SeedRange> destination_ranges;
        std::vector<SeedRange> source_ranges;
        for (auto [destination, source, range] : convert)
        {
            auto overlap = r.intersect(source, range);
            if (overlap.range > 0)
            {
                destination_ranges.push_back(
                    { .source = overlap.source + (destination - source), .range = overlap.range });
                source_ranges.push_back(overlap);
            }
        }

        // what ranges were not covered? sort and scan low to high
        // add mismatches directly to output
        std::sort(source_ranges.begin(), source_ranges.end(),
            [](auto a, auto b) { return a.source < b.source; });
        auto check = r.source;
        for (auto [source, range] : source_ranges)
        {
            if (source > check)
            {
                destination_ranges.push_back({ .source = check, .range = source - check });
            }
            check = source + range;
        }
        if (check < r.source + r.range)
        {
            destination_ranges.push_back({ .source = check, .range = r.source + r.range - check });
        }
        return destination_ranges;
    }

    uint64_t lookup(std::vector<Triple> convert, uint64_t src)
    {
        for (auto [destination, source, range] : convert)
        {
            if (source <= src && src <= source + range)
            {
                return src + (destination - source);
            }
        }
        return src;
    }

    static PromptInput from_file(const std::string& filepath)
    {
        std::ifstream file(filepath);
        auto lines = std::views::istream<FileReader>(file);

        auto seeds_view = lines | std::views::take(1);
        std::vector<uint64_t> seeds;
        // idk how to do this, so w.e this garbage is life now
        for (auto seed : seeds_view)
        {
            auto line = seed.line | std::views::split(' ')
                | std::views::transform([](auto&& v) { return std::string(v.begin(), v.end()); })
                | std::views::filter(
                    [](auto&& s) { return s.find(":") == std::string::npos && s.size() != 1; })
                | std::views::transform([](auto&& s) { return std::stoull(s); });
            seeds = std::vector<uint64_t>(line.begin(), line.end());
        }

        std::vector<std::vector<Triple>> conversions { {} };
        // unpacking FileReader
        for (auto&& [line] : lines)
        {
            if (line.empty())
            {
                conversions.push_back({});
            }
            else if (std::isdigit(line[0]))
            {
                auto nums_view = line | std::views::split(' ')
                    | std::views::transform(
                        [](auto&& v) { return std::string(v.begin(), v.end()); })
                    | std::views::transform([](auto&& s) { return std::stoull(s); });
                std::vector<uint64_t> nums(nums_view.begin(), nums_view.end());
                conversions.back().push_back({ nums[0], nums[1], nums[2] });
            }
        }
        return { .seeds = seeds, .conversions = conversions };
    }
};

void part1(const std::string& s)
{
    auto input = PromptInput::from_file(s);
    for (auto conversion : input.conversions)
    {
        for (int i = 0; i < input.seeds.size(); i++)
        {
            input.seeds[i] = input.lookup(conversion, input.seeds[i]);
        }
    }

    uint64_t min = UINT64_MAX;
    for (auto seed : input.seeds)
    {
        min = std::min(min, seed);
    }
    std::cout << "Lowest: " << min << std::endl;
}

void part2(const std::string& s)
{
    auto input = PromptInput::from_file(s);
    std::vector<SeedRange> seed_ranges;
    uint64_t base;
    for (size_t seed_index = 0; seed_index < input.seeds.size(); seed_index++)
    {
        if (seed_index % 2 == 0)
        {
            base = input.seeds[seed_index];
        }
        else
        {
            seed_ranges.push_back({ base, input.seeds[seed_index] });
        }
    }

    for (auto conversion : input.conversions)
    {
        std::vector<SeedRange> temp;
        for (int i = 0; i < seed_ranges.size(); i++)
        {
            for (auto rng : input.lookup(conversion, seed_ranges[i]))
            {
                temp.push_back(rng);
            }
        }
        seed_ranges = temp;
        std::sort(seed_ranges.begin(), seed_ranges.end(),
            [](auto a, auto b) { return a.source < b.source; });
    }

    uint64_t min = UINT64_MAX;
    for (auto seed : seed_ranges)
    {
        if (seed.source == 0)
            continue;
        min = std::min(min, seed.source);
    }
    std::cout << "Lowest: " << min << std::endl;
}
}

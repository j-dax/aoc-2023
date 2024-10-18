#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>

namespace day4
{

struct Card
{
    int id;
    std::vector<int> winning_numbers;
    std::vector<int> given_numbers;

    int matches;
};

std::vector<int> stoi_vec(const std::string& s)
{
    auto view = s | std::views::split(' ')
        | std::views::transform(
            [](auto&& num_str)
            {
                auto s_view = num_str | std::views::drop_while(isspace);
                std::string s = std::string(s_view.begin(), s_view.end());
                if (s.empty())
                    return -1;
                return std::stoi(s.data());
            })
        | std::views::filter([](auto&& n) { return n != -1; });

    return std::vector<int>(view.begin(), view.end());
}

int matches(const std::vector<int>& winning_numbers, const std::vector<int>& given_numbers)
{
    int matches_found = 0;

    for (auto given_num : given_numbers)
    {
        if (std::find(winning_numbers.begin(), winning_numbers.end(), given_num)
            != winning_numbers.end())
        {
            matches_found++;
        }
    }

    return matches_found;
}

int score(int matched)
{
    if (matched == 0)
        return 0;
    return 1 << (matched - 1);
}

Card line_to_card(const std::string& line)
{
    // Card <###>: <win> | <given>
    size_t colon_index = line.find_first_of(":");
    if (colon_index == std::string::npos)
        return Card { .id = -1 };

    std::string id_str = line.substr(5, colon_index);
    int id = std::stoi(id_str);

    size_t pipe_index = line.find_first_of("|");
    std::string win_str = line.substr(colon_index + 2, pipe_index - colon_index - 3);
    std::vector<int> winning_nums = stoi_vec(win_str);

    std::string given_str = line.substr(pipe_index + 2);
    std::vector<int> given_nums = stoi_vec(given_str);

    int card_matches = matches(winning_nums, given_nums);

    return Card { .id = id,
        .winning_numbers = winning_nums,
        .given_numbers = given_nums,
        .matches = card_matches };
}

std::ostream& operator<<(std::ostream& os, const std::vector<int>& v)
{
    os << "[ ";
    for (auto vi : v)
        os << vi << " ";
    os << " ]";
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<Card>& v)
{
    os << "[ ";
    for (auto vi : v)
        os << vi.matches << " ";
    os << " ]";
    return os;
}

void part1(const std::string& filename)
{
    std::ifstream file(filename);
    auto score_view =
        std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>())
        | std::views::split('\n')
        | std::views::transform([](auto&& card_str)
            { return line_to_card(std::string(card_str.begin(), card_str.end())); })
        | std::views::filter([](auto&& c) { return c.id != -1; });

    uint64_t sum = 0;
    for (auto card : score_view)
    {
        sum += score(card.matches);
    }

    std::cout << "Total: " << sum << std::endl;
}
void part2(const std::string& filename)
{
    std::ifstream file(filename);
    auto card_view =
        std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>())
        | std::views::split('\n')
        | std::views::transform([](auto&& card_str)
            { return line_to_card(std::string(card_str.begin(), card_str.end())); })
        | std::views::filter([](auto&& c) { return c.id != -1; });

    std::vector<Card> card_ref(card_view.begin(), card_view.end());
    std::vector<int> card_count(card_ref.size(), 1);

    // start at the beginning and carry down how many cards we'll get from one win of each
    for (int i = 0; i < card_count.size(); i++)
    {
        for (int win = 1 + i; win <= card_ref[i].matches + i; win++)
        {
            if (win < card_count.size())
            {
                card_count[win] += card_count[i];
            }
        }
    }

    // we win from each card once initially.
    size_t sum = std::accumulate(card_count.begin(), card_count.end(), 0);
    std::cout << "Total: " << sum << std::endl;
}
}

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string_view>
#include <vector>

namespace day7
{
// some behavior changes in the comparator in part two
// this should be flipped as needed
bool PART2 = false;

enum HandType
{
    HIGH_CARD = 0,
    ONE_PAIR = 1,
    TWO_PAIR = 2,
    THREE_OF_A_KIND = 3,
    FULL_HOUSE = 4,
    FOUR_OF_A_KIND = 5,
    FIVE_OF_A_KIND = 6,
};

struct Hand
{
    HandType type;
    std::string hand;
    int bid;

    bool operator<(const Hand& other) const
    {
        if (type != other.type)
        {
            return type < other.type;
        }
        for (int i = 0; i < 5; i++)
        {
            int thisScore = single_card_score(hand[i]);
            int otherScore = single_card_score(other.hand[i]);
            if (thisScore != otherScore)
                return thisScore < otherScore;
        }
        return false;
    }

    static int single_card_score(char c)
    {
        const std::string ranks = PART2 ? "J23456789TQKA" : "23456789TJQKA";
        return ranks.find(c);
    }

    static HandType get_type(std::string_view line_view)
    {
        std::map<char, int> card_counts;
        for (char c : line_view)
        {
            card_counts[c]++;
        }

        int J = 0;
        if (PART2)
        {
            J = card_counts['J'];
            card_counts['J'] = 0;
        }

        std::vector<int> counts;
        for (auto& [first, second] : card_counts)
        {
            counts.push_back(second);
        }
        std::sort(counts.begin(), counts.end(), std::greater<int>());

        switch (counts[0] + J)
        {
            case 5:
                return FIVE_OF_A_KIND;
            case 4:
                return FOUR_OF_A_KIND;
            case 3:
                return counts[1] == 2 ? FULL_HOUSE : THREE_OF_A_KIND;
            case 2:
                return counts[1] == 2 ? TWO_PAIR : ONE_PAIR;
            default:
                return HIGH_CARD;
        }
    }
};

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

std::ostream& operator<<(std::ostream& os, Hand h)
{
    const std::vector<std::string> types = {
        "HIGH_CARD",
        "ONE_PAIR",
        "TWO_PAIR",
        "THREE_OF_A_KIND",
        "FULL_HOUSE",
        "FOUR_OF_A_KIND",
        "FIVE_OF_A_KIND",
    };
    return os << std::left << std::setw(6) << h.hand << std::setw(4) << h.bid << std::setw(13)
              << types[h.type] << std::endl;
}

std::vector<Hand> read_file(const std::string& filepath)
{
    std::vector<Hand> hands;
    std::ifstream file(filepath);
    std::string hand;
    int bid;
    while (file >> hand >> bid)
    {
        hands.push_back({ Hand::get_type(hand), hand, bid });
    }
    return hands;
}

void part1(const std::string& filepath)
{
    auto hands = read_file(filepath);
    std::sort(hands.begin(), hands.end(), std::less<Hand>());
    uint64_t sum = 0;
    for (size_t idx = 0; idx < hands.size(); idx++)
    {
        sum += hands[idx].bid * (idx + 1);
    }
    std::cout << "Total: " << sum << std::endl;
}
void part2(const std::string& filepath)
{
    PART2 = true;
    auto hands = read_file(filepath);
    std::sort(hands.begin(), hands.end(), std::less<Hand>());
    uint64_t sum = 0;
    for (size_t idx = 0; idx < hands.size(); idx++)
    {
        sum += hands[idx].bid * (idx + 1);
    }
    std::cout << hands << std::endl;
    std::cout << "Total: " << sum << std::endl;
}
};

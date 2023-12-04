#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "HelpfulInfrastructure.h"

using namespace std; // I wouldn't normally do this
using namespace std::literals;

#pragma warning (disable: 4267) // size_t <-> uint
#pragma warning (disable: 4189) // unused variable
#pragma warning (disable: 4100) // unused param

std::unordered_set<int> NumFromStr(string const& value)
{
    std::unordered_set<int> result;

    std::istringstream istream{value};
    std::string input;
    while (std::getline(istream, input, ' '))
    {
        if (input.empty()) continue;
        result.emplace(std::atoi(input.c_str()));
    }
    return result;
}

void Part1()
{
    uint32_t result{};

    std::string input;
    while (std::getline(std::cin, input))
    {
        const auto colon = input.find(':');
        const auto bar = input.find('|');

        const auto winnersStr = input.substr(colon + 2, bar - colon - 2);
        const auto winners = NumFromStr(winnersStr);

        const auto mineStr = input.substr(bar + 2);
        const auto mine = NumFromStr(mineStr);

        uint32_t myWin{};
        for (const auto& num : mine)
        {
            if (winners.find(num) != winners.end())
            {
                ++myWin;
            }
        }

        if (myWin != 0)
        {
            result += (1 << (myWin - 1));
        }
    }
    std::cout << result << std::endl;
}

void Part2()
{
    struct Card
    {
        int CardNum{};
        std::unordered_set<int> Winners;
        std::unordered_set<int> Mine;
        int NumWinners{};
        int NumCopies{1};
    };
    std::vector<Card> cards;

    std::string input;
    while (std::getline(std::cin, input))
    {
        const auto colon = input.find(':');
        const auto bar = input.find('|');

        const auto winnersStr = input.substr(colon + 2, bar - colon - 2);
        const auto winners = NumFromStr(winnersStr);

        const auto mineStr = input.substr(bar + 2);
        const auto mine = NumFromStr(mineStr);

        int myWin{};
        for (const auto& num : mine)
        {
            if (winners.find(num) != winners.end())
            {
                ++myWin;
            }
        }

        Card card { (int)cards.size() + 1, std::move(winners), std::move(mine), myWin };
        cards.emplace_back(std::move(card));
    }

    for (auto i = 0; i < cards.size(); ++i)
    {
        const auto& card = cards[i];
        for (auto winners = 0; winners < card.NumWinners; ++winners)
        {
            const auto index = i + winners + 1;
            if (index < cards.size())
            {
                cards[index].NumCopies += card.NumCopies;
            }
        }
    }

    uint32_t result = std::accumulate(cards.begin(), cards.end(), 0UL, [](uint32_t total, Card const& c) { return total + c.NumCopies; });
    std::cout << result << std::endl;
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
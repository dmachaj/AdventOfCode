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

uint32_t FindJoltage(const std::string& line)
{
    std::vector<int> items;
    for (auto i : line)
    {
        items.push_back(i - '0');
    }

    uint32_t max{};
    for (auto i = 0; i < (items.size() - 1); ++i)
    {
        for (auto j = i + 1; j < items.size(); ++j)
        {
            uint32_t num = (items[i] * 10) + items[j];
            if (num > max)
            {
                max = num;
            }
        }
    }
    return max;
}

void Part1()
{
    uint64_t result{};

    std::string input;
    while (std::getline(std::cin, input))
    {
        result += FindJoltage(input);
    }
    std::cout << result << std::endl;
}

uint64_t FindJoltagePart2(const std::string& line)
{
    std::vector<int> items;
    for (auto i : line)
    {
        items.push_back(i - '0');
    }

    std::string digits;
    int minIndex = 0;
    for (auto i = 0; i < 12; ++i)
    {
        // This was the hardest part to get right.  This is a greedy algorithm so we can search up to
        // one more than the number of skipped items.  But as earlier greedy steps skip over items that
        // leaves less room to later iterations to pick bigger numbers.  In a lot of cases the last
        // few iterations will have no choice and will check exactly one digit.
        const int digitsToBeChecked = (items.size() - minIndex) - (12 - i - 1);

        int biggestDigit = 0;
        int biggestDigitIndex = -1;
        for (auto j = minIndex; j < (minIndex + digitsToBeChecked); ++j)
        {
            if (j >= items.size()) { throw std::exception(); }

            if (items[j] > biggestDigit)
            {
                biggestDigit = items[j];
                biggestDigitIndex = j;
            }
        }

        minIndex = biggestDigitIndex + 1;
        digits += (char)biggestDigit + '0';
    }

    return std::atoll(digits.c_str());
}

void Part2()
{
    uint64_t result{};

    std::string input;
    while (std::getline(std::cin, input))
    {
        const auto joltage = FindJoltagePart2(input);
        result += joltage;
        // std::cerr << joltage << std::endl;
    }
    std::cout << result << std::endl;
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
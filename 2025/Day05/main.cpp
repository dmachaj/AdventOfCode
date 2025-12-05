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

void Part1()
{
    std::vector<std::pair<uint64_t, uint64_t>> fresh;

    std::string input;
    while (std::getline(std::cin, input))
    {
        if (input.empty()) break;

        const auto dash = input.find('-');
        const auto low = std::atoll(input.substr(0, dash).c_str());
        const auto high = std::atoll(input.substr(dash + 1).c_str());

        fresh.push_back(std::make_pair(low, high));
    }

    uint64_t result{};

    while (std::getline(std::cin, input))
    {
        const uint64_t num = std::atoll(input.c_str());
        for (auto& item : fresh)
        {
            if ((num >= item.first) && (num <= item.second))
            {
                ++result;
                break;
            }
        }
    }

    std::cout << result << std::endl;
}

bool RangesIntersect(std::pair<uint64_t, uint64_t> const& left, std::pair<uint64_t, uint64_t> const& right)
{
    // const auto leftMin = (left.first >= right.first) && (left.first <= right.second);
    // const auto leftMax = (left.second >= right.first) && (left.second <= right.second);
    // const auto rightMin = (right.first >= left.first) && (right.first <= left.second);
    // const auto rightMax = (right.second >= left.first) && (right.second <= left.second);
    // const auto leftSurrounds = (left.first <= right.first) && (left.second >= right.second);
    // return leftMin || leftMax || rightMin || rightMax;

    return std::max(left.first, right.first) <= std::min(left.second, right.second);
}

void Part2()
{
    std::vector<std::pair<uint64_t, uint64_t>> fresh;

    std::string input;
    while (std::getline(std::cin, input))
    {
        if (input.empty()) break;

        const auto dash = input.find('-');
        const auto low = std::atoll(input.substr(0, dash).c_str());
        const auto high = std::atoll(input.substr(dash + 1).c_str());

        fresh.push_back(std::make_pair(low, high));
    }

    // Keep looping and combining ranges until there is nothing left to combine.
    bool dirty = true;
    while (dirty)
    {
        dirty = false;

        std::vector<std::pair<uint64_t, uint64_t>> freshMin;
        for (auto& item : fresh)
        {
            bool intersected = false;
            for (auto& j : freshMin)
            {
                if (RangesIntersect(item, j))
                {
                    j.first = std::min(item.first, j.first);
                    j.second = std::max(item.second, j.second);
                    intersected = true;
                    dirty = true;
                    break;
                }
            }

            if (!intersected)
            {
                freshMin.emplace_back(item);
            }
        }

        std::swap(freshMin, fresh);
    }

    // for (auto& item : fresh)
    // {
    //     std::cerr << item.first << " " << item.second << std::endl;
    // }

    uint64_t result = std::accumulate(fresh.begin(), fresh.end(), 0ULL, [](uint64_t sum, auto item) { return sum + (item.second - item.first + 1); });
    std::cout << result << std::endl;
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
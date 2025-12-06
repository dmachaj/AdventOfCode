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
    std::vector<std::string> lines;

    std::string input;
    while (std::getline(std::cin, input))
    {
        lines.emplace_back(std::move(input));
    }

    auto lastStartingSpot = 0;
    std::vector<std::vector<uint64_t>> nums(lines.size() - 1);
    std::vector<bool> ops; // true is mult, false is add

    for (auto i = 0; i < lines[0].size(); ++i)
    {
        bool isBreak = std::all_of(lines.begin(), lines.end(), [i](std::string const& str) { return str[i] == ' '; });
        bool isEnd = (i == lines[0].size() - 1);
        if (!isBreak && !isEnd) continue;

        for (auto j = 0; j < (lines.size() - 1); ++j)
        {
            const auto numStr = isEnd ? lines[j].substr(lastStartingSpot) : lines[j].substr(lastStartingSpot, (i - lastStartingSpot));
            nums[j].emplace_back(std::atoll(numStr.c_str()));
        }

        const char op = lines[lines.size() - 1][lastStartingSpot] == '*';
        ops.emplace_back(op);

        lastStartingSpot = i + 1;
    }

    uint64_t result{};
    for (auto i = 0; i < nums[0].size(); ++i)
    {
        uint64_t temp = nums[0][i];
        for (auto j = 1; j < nums.size(); ++j)
        {
            if (ops[i]) // mult
            {
                temp *= nums[j][i];
            }
            else
            {
                temp += nums[j][i];
            }
        }

        result += temp;
    }

    std::cout << result << std::endl;
}

void Part2()
{
    std::vector<std::string> lines;

    std::string input;
    while (std::getline(std::cin, input))
    {
        lines.emplace_back(std::move(input));
    }

    auto lastStartingSpot = 0;
    uint64_t result{};

    for (auto i = 0; i < lines[0].size(); ++i)
    {
        bool isBreak = std::all_of(lines.begin(), lines.end(), [i](std::string const& str) { return str[i] == ' '; });
        bool isEnd = (i == lines[0].size() - 1);
        if (!isBreak && !isEnd) continue;

        if (isEnd) i += 1; // hack to fix last column

        std::vector<int> nums;
        for (auto col = lastStartingSpot; col < i; ++col)
        {
            std::string numStr;
            for (auto row = 0; row < lines.size() - 1; ++row)
            {
                numStr += lines[row][col];
            }

            nums.emplace_back(std::atoi(numStr.c_str()));
        }

        bool op = lines[lines.size() - 1][lastStartingSpot] == '*';

        uint64_t temp = nums[0];
        for (auto j = 1; j < nums.size(); ++j)
        {
            if (op) // mult
            {
                temp *= nums[j];
            }
            else
            {
                temp += nums[j];
            }
        }
        result += temp;

        lastStartingSpot = i + 1;
    }

    std::cout << result << std::endl;
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <optional>
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

int64_t FindPivot(std::vector<std::string> const& input, int64_t filter = 0)
{
    for (auto pivot = 1; pivot < input.size(); ++pivot)
    {
        if (pivot == filter) continue;

        const auto numToCompare = std::min(pivot, (int)input.size() - pivot);
        const auto upIter = input.rbegin() + (input.size() - pivot);
        const auto downIter = input.begin() + pivot;
        if (std::equal(upIter, upIter + numToCompare, downIter, downIter + numToCompare))
        {
            return pivot;
        }
    }
    return 0;
}

int64_t FindSingleMirror(std::vector<std::string> const& input)
{
    // Check for vertical mirroring first because it is faster/easier
    const auto verticalPivot = FindPivot(input);
    if (verticalPivot != 0)
    {
        return verticalPivot * 100;
    }

    const int width = input[0].size();
    std::vector<std::string> rotated;
    for (auto i = 0; i < width; ++i)
    {
        std::string line;
        for (auto j = 0; j < input.size(); ++j)
        {
            line += input[j][i];
        }
        rotated.emplace_back(std::move(line));
    }

    const auto horizontalPivot = FindPivot(rotated);

    if (verticalPivot == 0 && horizontalPivot == 0) throw std::exception();

    return horizontalPivot;
}

void Part1()
{
    std::vector<std::string> problem;
    int64_t result{};

    std::string input;
    while (std::getline(std::cin, input))
    {
        if (input.empty())
        {
            result += FindSingleMirror(problem);
            problem.clear();
            continue;
        }

        problem.emplace_back(std::move(input));
    }

    result += FindSingleMirror(problem);

    std::cout << result << std::endl;
}

using HorizVertResult = std::pair<int64_t, int64_t>;

std::optional<HorizVertResult> FindSingleMirror2(std::vector<std::string> const& input, HorizVertResult filter)
{
    const auto verticalPivot = FindPivot(input, filter.second);
    if (!verticalPivot)
    {
        const int width = input[0].size();
        std::vector<std::string> rotated;
        for (auto i = 0; i < width; ++i)
        {
            std::string line;
            for (auto j = 0; j < input.size(); ++j)
            {
                line += input[j][i];
            }
            rotated.emplace_back(std::move(line));
        }

        const auto horizontalPivot = FindPivot(rotated, filter.first);
        if (horizontalPivot)
        {
            return make_pair(horizontalPivot, 0);
        }
        return nullopt;
    }

    return make_pair(0, verticalPivot);
}

int64_t FindMirrorPermutation(std::vector<std::string> const& input)
{
    const auto originalResult = FindSingleMirror2(input, make_pair(0, 0)).value();

    const auto width = input[0].size();
    const auto height = input.size();
    for (auto i = 0; i < (width * height); ++i)
    {
        auto copy = input;
        const auto row = i / width;
        const auto col = i % width;

        auto& val = copy[row][col];
        if (val == '#') val = '.';
        else val = '#';

        if (auto result = FindSingleMirror2(copy, originalResult); result.has_value())
        {
            return (result.value().first + result.value().second * 100);
        }
    }
    
    throw std::exception(); // no match found
}

void Part2()
{
    std::vector<std::string> problem;
    int64_t result{};

    std::string input;
    while (std::getline(std::cin, input))
    {
        if (input.empty())
        {
            result += FindMirrorPermutation(problem);
            problem.clear();
            continue;
        }

        problem.emplace_back(std::move(input));
    }

    result += FindMirrorPermutation(problem);

    std::cout << result << std::endl;
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
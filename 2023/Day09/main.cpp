#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <stack>
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

int64_t SolvePart1(std::string const& input)
{
    std::vector<int64_t> numbers;

    std::istringstream inStream(input);
    std::string temp;
    while (std::getline(inStream, temp, ' '))
    {
        numbers.emplace_back(std::strtoll(temp.c_str(), nullptr, 10));
    }

    const auto doneLambda = [](const std::vector<int64_t>& val)
    {
        return std::all_of(val.begin(), val.end(), [](int64_t inner) { return inner == 0; });
    };

    std::stack<std::vector<int64_t>> lines;
    lines.emplace(numbers);

    while (!doneLambda(lines.top()))
    {
        const auto& top = lines.top();

        std::vector<int64_t> nextLine;
        for (auto i = 0; i < top.size() - 1; ++i)
        {
            const auto diff = top[i + 1] - top[i];
            nextLine.emplace_back(diff);
        }
        lines.emplace(std::move(nextLine));
    }

    int64_t endOfPreviousLine{};
    while (!lines.empty())
    {
        const auto& top = lines.top();
        int64_t previousValue = top[top.size() - 1];
        int64_t nexVal = endOfPreviousLine + previousValue;
        endOfPreviousLine = nexVal;
        lines.pop();
    }

    return endOfPreviousLine;
}

void Part1()
{
    int64_t result{};

    std::string input;
    while (std::getline(std::cin, input))
    {
        result += SolvePart1(input);
    }
    std::cout << result << std::endl;
}

int64_t SolvePart2(std::string const& input)
{
    std::vector<int64_t> numbers;

    std::istringstream inStream(input);
    std::string temp;
    while (std::getline(inStream, temp, ' '))
    {
        numbers.emplace_back(std::strtoll(temp.c_str(), nullptr, 10));
    }

    const auto doneLambda = [](const std::vector<int64_t>& val)
    {
        return std::all_of(val.begin(), val.end(), [](int64_t inner) { return inner == 0; });
    };

    std::stack<std::vector<int64_t>> lines;
    lines.emplace(numbers);

    while (!doneLambda(lines.top()))
    {
        const auto& top = lines.top();

        std::vector<int64_t> nextLine;
        for (auto i = 0; i < top.size() - 1; ++i)
        {
            const auto diff = top[i + 1] - top[i];
            nextLine.emplace_back(diff);
        }
        lines.emplace(std::move(nextLine));
    }

    int64_t beginningOfPreviousLine{};
    while (!lines.empty())
    {
        const auto& top = lines.top();
        int64_t previousValue = top[0];
        int64_t nexVal = previousValue - beginningOfPreviousLine;
        beginningOfPreviousLine = nexVal;
        lines.pop();
    }

    return beginningOfPreviousLine;
}

void Part2()
{
    int64_t result{};

    std::string input;
    while (std::getline(std::cin, input))
    {
        result += SolvePart2(input);
    }
    std::cout << result << std::endl;
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
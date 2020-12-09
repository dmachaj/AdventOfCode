#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include "HelpfulInfrastructure.h"

using namespace std; // I wouldn't normally do this
using namespace std::literals;

#pragma warning (disable: 4267) // size_t <-> uint

namespace
{
    uint32_t SolvePart1(uint32_t preambleSize, const std::vector<uint32_t>& numbers)
    {
        uint32_t windowMin{};
        uint32_t windowMax{preambleSize - 1};
        for (auto i = preambleSize; i < numbers.size(); ++i)
        {
            uint32_t target = numbers[i];
            bool found{};

            for (auto k = windowMin; !found && k <= windowMax; ++k)
            {
                for (auto j = (k + 1); !found && j <= windowMax; ++j)
                {
                    if (numbers[k] + numbers[j] == target)
                    {
                        found = true;
                    }
                }
            }

            if (!found)
            {
                return target;
            }

            windowMin++;
            windowMax++;
        }
        return 0;
    }

    void Part1()
    {
        uint32_t preambleSize;
        std::cin >> preambleSize;
        std::cin.get(); // eat newline 

        std::vector<uint32_t> numbers{};
        std::string input;
        while (std::getline(std::cin, input))
        {
            uint32_t num = std::atoi(input.c_str());
            numbers.push_back(num);
        }

        std::cout << SolvePart1(preambleSize, numbers) << std::endl;
    }

    void Part2()
    {
        uint32_t preambleSize;
        std::cin >> preambleSize;
        std::cin.get(); // eat newline 

        std::vector<uint32_t> numbers{};
        std::string input;
        while (std::getline(std::cin, input))
        {
            uint32_t num = std::atoi(input.c_str());
            numbers.push_back(num);
        }

        const auto target = SolvePart1(preambleSize, numbers);

        for (auto i = numbers.begin(); i != numbers.end(); ++i)
        {
            for (auto j = i + 1; j != numbers.end(); ++j)
            {
                const auto sum = std::accumulate(i, j, 0UL);
                if (sum == target)
                {
                    const auto lower = *std::min_element(i, j);
                    const auto upper = *std::max_element(i, j);
                    std::cout << (lower + upper) << std::endl;
                    return;
                }
                else if (sum > target)
                {
                    break;
                }
            }
        }
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
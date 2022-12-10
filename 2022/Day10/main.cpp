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

namespace
{
    bool CycleCountMatters(uint32_t cycleCount)
    {
        return (cycleCount == 20) || (cycleCount == 60) || (cycleCount == 100) || (cycleCount == 140) || (cycleCount == 180) || (cycleCount == 220);
    }

    void Part1()
    {
        uint32_t cycleCount{};
        int32_t value{1};
        uint32_t finalValue{};

        std::string input;
        while (std::getline(std::cin, input))
        {
            if (input == "noop")
            {
                ++cycleCount;
                if (CycleCountMatters(cycleCount))
                {
                    finalValue += (cycleCount * value);
                    std::cerr << (cycleCount * value) << std::endl;
                }
            }
            else
            {
                int32_t offset = std::atoi(input.substr(input.find(' ') + 1).c_str());

                ++cycleCount;
                if (CycleCountMatters(cycleCount))
                {
                    finalValue += (cycleCount * value);
                    std::cerr << (cycleCount * value) << std::endl;
                }

                ++cycleCount;
                if (CycleCountMatters(cycleCount))
                {
                    finalValue += (cycleCount * value);
                    std::cerr << (cycleCount * value) << std::endl;
                }
                value += offset;
            }
        }
        std::cout << finalValue << std::endl;
    }

    void TryRender(vector<char>& screen, uint32_t cycleCount, uint32_t value, uint32_t columnCount)
    {
        const auto column = cycleCount % columnCount;
        if ((value -1 == column) || (value == column) || (value + 1 == column))
        {
            screen[cycleCount] = '#';
        }
    }

    void Part2()
    {
        constexpr uint32_t c_width = 40;
        constexpr uint32_t c_height = 6;

        uint32_t cycleCount{};
        int32_t value{1};
        vector<char> screen(c_width * c_height, '.');

        std::string input;
        while (std::getline(std::cin, input))
        {
            if (input == "noop")
            {
                TryRender(screen, cycleCount, value, c_width);
                ++cycleCount;
            }
            else
            {
                int32_t offset = std::atoi(input.substr(input.find(' ') + 1).c_str());

                TryRender(screen, cycleCount, value, c_width);
                ++cycleCount;

                TryRender(screen, cycleCount, value, c_width);
                ++cycleCount;
                value += offset;
            }
        }

        for (auto i = 0UL; i < c_height; ++i)
        {
            for (auto j = (i * c_width); j < ((i + 1) * c_width); ++j)
            {
                std::cout << screen[j];
            }
            std::cout << std::endl;
        }
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_set>
#include "HelpfulInfrastructure.h"
#include "../Intcode/Intcode.h"

namespace
{
    constexpr size_t c_width{100};
    constexpr size_t c_height{100};

    void Part1()
    {
        auto state = Intcode::ParseProgram();

        std::array<bool, c_width * c_height> panel{false}; // false = black, true = white
        std::array<bool, c_width * c_height> visitedPanels{false};
        std::pair<uint32_t, uint32_t> robotPosition{50, 50};
        int32_t robotDirection{0}; // 0 = up, 1 = right, 2 = down, 3 = left

        auto programInput = Intcode::CallbackIntcodeInput([&robotPosition, &panel]()
        {
            const auto currentIndex = (robotPosition.second * c_width) + robotPosition.first;
            const auto currentColor = panel[currentIndex];
            return currentColor ? 1ULL : 0ULL;
        });

        bool nextOutputIsColor{true};
        auto programOutput = Intcode::CallbackIntcodeOutput([&panel, &visitedPanels, &robotDirection, &robotPosition, &nextOutputIsColor](int64_t value)
        {
            if (nextOutputIsColor)
            {
                const auto currentIndex = (robotPosition.second * c_width) + robotPosition.first;
                panel[currentIndex] = (value == 1ULL);
                visitedPanels[currentIndex] = true;
            }
            else
            {
                if (value == 1) // turn right
                {
                    robotDirection++;
                    robotDirection %= 4;
                }
                else
                {
                    robotDirection--;
                    if (robotDirection < 0)
                    {
                        robotDirection = 3;
                    }
                }
                
                switch (robotDirection)
                {
                    case 0: // up
                        robotPosition.second--;
                        break;
                    case 1: // right
                        robotPosition.first++;
                        break;
                    case 2: // down
                        robotPosition.second++;
                        break;
                    case 3: // left
                        robotPosition.first--;
                        break;
                    default:
                        throw std::exception("oops");
                }
            }
            nextOutputIsColor = !nextOutputIsColor;
        });

        ExecuteProgram(state, &programInput, &programOutput);
        const uint32_t visitedPositions = std::accumulate(visitedPanels.begin(), visitedPanels.end(), 0UL, [](uint32_t sum, bool visited) { return visited ? (sum + 1) : sum; });
        std::cout << visitedPositions << std::endl;
    }

    void Part2()
    {
        auto state = Intcode::ParseProgram();

        std::array<bool, c_width * c_height> panel{false}; // false = black, true = white
        std::array<bool, c_width * c_height> visitedPanels{false};
        std::pair<uint32_t, uint32_t> robotPosition{50, 50};
        int32_t robotDirection{0}; // 0 = up, 1 = right, 2 = down, 3 = left

        panel[(robotPosition.second * c_width) + robotPosition.first] = true;

        auto programInput = Intcode::CallbackIntcodeInput([&robotPosition, &panel]()
        {
            const auto currentIndex = (robotPosition.second * c_width) + robotPosition.first;
            const auto currentColor = panel[currentIndex];
            return currentColor ? 1ULL : 0ULL;
        });

        bool nextOutputIsColor{true};
        auto programOutput = Intcode::CallbackIntcodeOutput([&panel, &visitedPanels, &robotDirection, &robotPosition, &nextOutputIsColor](int64_t value)
        {
            if (nextOutputIsColor)
            {
                const auto currentIndex = (robotPosition.second * c_width) + robotPosition.first;
                panel[currentIndex] = (value == 1ULL);
                visitedPanels[currentIndex] = true;
            }
            else
            {
                if (value == 1) // turn right
                {
                    robotDirection++;
                    robotDirection %= 4;
                }
                else
                {
                    robotDirection--;
                    if (robotDirection < 0)
                    {
                        robotDirection = 3;
                    }
                }
                
                switch (robotDirection)
                {
                    case 0: // up
                        robotPosition.second--;
                        break;
                    case 1: // right
                        robotPosition.first++;
                        break;
                    case 2: // down
                        robotPosition.second++;
                        break;
                    case 3: // left
                        robotPosition.first--;
                        break;
                    default:
                        throw std::exception("oops");
                }
            }
            nextOutputIsColor = !nextOutputIsColor;
        });

        Intcode::ExecuteProgram(state, &programInput, &programOutput);

        for (auto x = 0UL; x < c_width; ++x)
        {
            for (auto y = 0UL; y < c_height; ++y)
            {
                std::cout << (panel[(y * c_width) + x] ? "#" : ".");
            }
            std::cout << std::endl;
        }
        // ZCGRHKLB is spelled out in # and .
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
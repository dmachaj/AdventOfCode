#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "HelpfulInfrastructure.h"

using namespace std::literals;

#pragma warning (disable: 4267) // size_t <-> uint

namespace
{
    uint32_t ParseTicket(std::string input)
    {
        uint32_t lower{};
        uint32_t upper{128};
        uint32_t index{};

        while (upper - lower > 1)
        {
            char letter = input[index];
            if (letter == 'F')
            {
                upper = ((upper - lower)) / 2 + lower;
            }
            else if (letter == 'B')
            {
                lower = ((upper - lower)) / 2  + lower;
            }
            else
            {
                throw 0;
            }
            ++index;
        }

        uint32_t colMin{};
        uint32_t colMax{8};
        while (colMax - colMin > 1)
        {
            char letter = input[index];
            if (letter == 'L')
            {
                colMax = ((colMax - colMin) / 2) + colMin;
            }
            else if (letter == 'R')
            {
                colMin = ((colMax - colMin) / 2) + colMin;
            }
            else
            {
                throw 0;
            }
            ++index;
        }

        const auto result = (lower * 8) + colMin;
        // std::cerr << result << std::endl;
        return result;
    }

    void Part1()
    {
        uint32_t highestBoardingPass{};

        std::string input;
        while (std::getline(std::cin, input))
        {
            const auto passId = ParseTicket(input);
            if (passId > highestBoardingPass) highestBoardingPass = passId;
        }
        std::cout << highestBoardingPass << std::endl;
    }

    void Part2()
    {
        std::vector<uint32_t> allSeats{};

        std::string input;
        while (std::getline(std::cin, input))
        {
            allSeats.emplace_back(ParseTicket(input));
        }

        std::sort(allSeats.begin(), allSeats.end());
        for (auto i = 1; i < allSeats.size(); ++i)
        {
            if ((allSeats[i] - allSeats[i - 1]) != 1)
            {
                std::cout << allSeats[i - 1] + 1 << std::endl;
            }
        }
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
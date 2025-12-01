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
    int currentPos{50};
    uint32_t timesItWasZero{};

    std::string input;
    while (std::getline(std::cin, input))
    {
        bool right = input[0] == 'R';
        const auto numStr = input.substr(1);
        const auto amount = std::atoi(numStr.c_str());

        // std::cerr << "Input: " << input << "\nIsRight: " << right << "\nAmount: " << amount << std::endl;

        int math = right ? amount : -amount;
        currentPos += math;

        currentPos = currentPos % 100;
        if (currentPos < 0)
        {
            currentPos = 100 + currentPos;
        }
        // std::cerr << "CurrentPos: " << currentPos << std::endl;

        if (currentPos == 0)
        {
            ++timesItWasZero;
        }
    }
    std::cout << timesItWasZero << std::endl;
}

void Part2()
{
    int currentPos{50};
    uint32_t timesCrossedZero{};

    std::string input;
    while (std::getline(std::cin, input))
    {
        bool right = input[0] == 'R';
        const auto numStr = input.substr(1);
        auto amount = std::atoi(numStr.c_str());
        const auto previousPos = currentPos;

        // std::cerr << "Input: " << input << "\nIsRight: " << right << "\nAmount: " << amount << std::endl;

        if (amount >= 100)
        {
            const auto spins = amount / 100;
            timesCrossedZero += spins;

            // std::cerr << "  Multiple spins: " << spins << std::endl;
            amount = amount % 100;
        }

        int math = right ? amount : -amount;
        currentPos += math;

        currentPos = currentPos % 100;
        if (currentPos < 0)
        {
            currentPos = 100 + currentPos;
        }
        // std::cerr << "CurrentPos: " << currentPos << std::endl;

        const bool overflowed = (right && currentPos < previousPos) && (previousPos != 0);
        const bool underflowed = !right && currentPos > previousPos && (previousPos != 0);
        const bool landedOnZero = currentPos == 0;
        if (overflowed || underflowed || landedOnZero)
        {
            // std::cerr << "  Crossed zero!" << std::endl;
            ++timesCrossedZero;
        }
    }

    std::cout << timesCrossedZero << std::endl;
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
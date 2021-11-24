#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include "HelpfulInfrastructure.h"

void Part1()
{
    int64_t runningTotal{};
    std::string input;
    while (std::getline(std::cin, input))
    {
        const auto number = std::atoi(input.c_str());
        runningTotal += number;
        // std::cout << "Read in " << number << " running total " << runningTotal << std::endl;
    }
    std::cout << runningTotal << std::endl;
}

void Part2()
{
    std::string inputStr;
    std::vector<int64_t> inputSequence{};
    while (std::getline(std::cin, inputStr))
    {
        inputSequence.push_back(std::atoi(inputStr.c_str()));
    }

#if 0
    std::cerr << "Input sequence is ";
    for (auto input : inputSequence)
    {
        std::cerr << input << ", ";
    }
    std::cerr << std::endl;
#endif

    int64_t runningTotal{};
    uint64_t inputSequenceIndex{};
    std::set<int64_t> seenNumbers{0};
    while (true)
    {
        runningTotal += inputSequence[inputSequenceIndex];
        // std::cerr << "Read " << inputSequence[inputSequenceIndex] << " running total is " << runningTotal << std::endl;

        if (seenNumbers.find(runningTotal) != seenNumbers.end())
        {
            std::cout << runningTotal << std::endl;
            break;
        }
        seenNumbers.emplace(runningTotal);
        inputSequenceIndex = ++inputSequenceIndex % inputSequence.size();
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "HelpfulInfrastructure.h"

#pragma warning (disable: 4267) // size_t <-> uint

namespace
{
    void Part1()
    {
        std::vector<int> numbers{};
        std::string input;
        while (std::getline(std::cin, input))
        {
            numbers.emplace_back(std::atoi(input.c_str()));
        }

        for (auto i = 0; i < numbers.size(); ++i)
        {
            for (auto j = 0; j < numbers.size(); ++j)
            {
                if (i == j) continue;

                if ((numbers[i] + numbers[j]) == 2020)
                {
                    std::cout << (numbers[i] * numbers[j]) << std::endl;
                    return;
                }
            }
        }

        std::cout << "Dunno" << std::endl;
    }

    void Part2()
    {
        std::vector<int> numbers{};
        std::string input;
        while (std::getline(std::cin, input))
        {
            numbers.emplace_back(std::atoi(input.c_str()));
        }

        for (auto i = 0; i < numbers.size(); ++i)
        {
            for (auto j = 0; j < numbers.size(); ++j)
            {
                for (auto k = 0; k < numbers.size(); ++k)
                {
                    if (i == j) continue;
                    if (i == k) continue;
                    if (j == k) continue;

                    if ((numbers[i] + numbers[j] + numbers[k]) == 2020)
                    {
                        std::cout << (numbers[i] * numbers[j] * numbers[k]) << std::endl;
                        return;
                    }
                }
            }
        }

        std::cout << "Dunno" << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
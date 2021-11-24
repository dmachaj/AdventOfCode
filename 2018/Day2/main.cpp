#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include "HelpfulInfrastructure.h"

void Part1()
{
    std::string input;
    uint32_t countDoubles{};
    uint32_t countTriples{};
    while (std::getline(std::cin, input))
    {
        bool containsDouble{};
        bool containsTriple{};
        for (auto letter = 'a'; letter <= 'z'; ++letter)
        {
            const auto count = std::count_if(input.begin(), input.end(), [letter](auto inputLetter) -> bool { return inputLetter == letter; });
            if (count == 2)
            {
                containsDouble = true;
            }
            else if (count == 3)
            {
                containsTriple = true;
            }
        }

        if (containsDouble)
        {
            countDoubles++;
        }
        
        if (containsTriple)
        {
            countTriples++;
        }
    }

    std::cout << (countDoubles * countTriples) << std::endl;
}

void Part2()
{
    std::vector<std::string> allInputs{};
    std::string input;
    while (std::getline(std::cin, input))
    {
        std::cerr << "Input " << input << std::endl;
        allInputs.emplace_back(std::move(input));
    }

    bool found{};
    for (auto i = 0UL; (i < allInputs.size()) && !found; ++i)
    {
        for (auto j = (i + 1); (j < allInputs.size()) && !found; ++j)
        {
            uint32_t differences{};
            for (auto k = 0UL; k < allInputs[i].size(); ++k)
            {
                if (allInputs[i][k] != allInputs[j][k])
                {
                    differences++;
                }
            }

            if (differences == 1)
            {
                for (auto k = 0UL; k < allInputs[i].size(); ++k)
                {
                    if (allInputs[i][k] == allInputs[j][k])
                    {
                        std::cout << allInputs[i][k];
                    }
                }
                found = true;
            }
        }
    }
    
    std::cout << std::endl;
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
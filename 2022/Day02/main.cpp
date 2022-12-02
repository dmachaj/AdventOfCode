#include <algorithm>
#include <array>
#include <cmath>
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
#include <assert.h>

using namespace std; // I wouldn't normally do this
using namespace std::literals;

#pragma warning (disable: 4267) // size_t <-> uint
#pragma warning (disable: 4189) // unused variable
#pragma warning (disable: 4100) // unused param

namespace
{
    void Part1()
    {
        uint32_t total{};

        std::unordered_map<std::string, uint32_t> scores = {
            { "A X", (1 + 3) },
            { "A Y", (2 + 6) },
            { "A Z", (3 + 0) },
            { "B X", (1 + 0) },
            { "B Y", (2 + 3) },
            { "B Z", (3 + 6) },
            { "C X", (1 + 6) },
            { "C Y", (2 + 0) },
            { "C Z", (3 + 3) },
        };

        std::string input;
        while (std::getline(std::cin, input))
        {
            total += scores[input];
        }
        assert((scores.size() == 9UL));
        std::cout << total << std::endl;
    }

    void Part2()
    {
        uint32_t total{};

        std::string input;
        while (std::getline(std::cin, input))
        {
            if (input[0] == 'A') // they played rock
            {
                if (input[2] == 'X') // lose
                {
                    // scissors
                    total += (0 + 3);
                }
                else if (input[2] == 'Y') // draw
                {
                    // rock
                    total += (3 + 1);
                }
                else // win
                {
                    // paper
                    total += (6 + 2);
                }
            }
            else if (input[0] == 'B') // they played paper
            {
                if (input[2] == 'X') // lose
                {
                    // rock
                    total += (0 + 1);
                }
                else if (input[2] == 'Y') // draw
                {
                    // paper
                    total += (3 + 2);
                }
                else // win
                {
                    // scissors
                    total += (6 + 3);
                }
            }
            else if (input[0] == 'C') // they played scissors
            {
                if (input[2] == 'X') // lose
                {
                    // paper
                    total += (0 + 2);
                }
                else if (input[2] == 'Y') // draw
                {
                    // scissors
                    total += (3 + 3);
                }
                else // win
                {
                    // rock
                    total += (6 + 1);
                }
            }
        }

        std::cout << total << std::endl;;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
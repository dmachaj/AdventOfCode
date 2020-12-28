#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "HelpfulInfrastructure.h"
#include "../Intcode/Intcode.h"

namespace
{
    void Part1()
    {
        auto state = Intcode::ParseProgram();
        const auto result = Intcode::ExecuteProgram(state, {0ULL});

        std::vector<bool> view{};
        uint32_t width{};
        uint32_t counter{};
        for (const auto& letter : result)
        {
            if (letter == '#')
            {
                std::cerr << letter;
            }
            else if (letter == '.')
            {
                std::cerr << letter;
            }
            else if (letter == 10)
            {
                std::cerr << std::endl;
            }
            else if ((letter == '<') || (letter == '>') || (letter == '^') || (letter == 'v'))
            {
                std::cerr << "R";
            }
            else
            {
                throw std::exception("Unknown output");
            }

            if ((letter == '#') || (letter == '<') || (letter == '>') || (letter == '^') || (letter == 'v'))
            {
                view.emplace_back(true);
            }
            else if (letter == 10)
            {
                if (width == 0)
                {
                    width = counter;
                }
            }
            else
            {
                view.emplace_back(false);
            }

            counter++;
        }

        uint32_t resultTotal{};
        const auto height = view.size() / width;
        for (auto y = 0; y < (int32_t)width; ++y)
        {
            for (auto x = 0; x < (int32_t)height; ++x)
            {
                if (((x - 1) >= 0) && ((x + 1) < (int32_t)width) && ((y - 1) >= 0) && ((y + 1) < (int32_t)height))
                {
                    const auto center = view[y * width + x];
                    const auto above = view[(y-1) * width + x];
                    const auto left = view[y * width + (x-1)];
                    const auto right = view[y * width + (x+1)];
                    const auto below = view[(y+1) * width + x];

                    if (center && above && left && right && below)
                    {
                        std::cerr << "(" << x << "," << y << ")" << std::endl;
                        resultTotal += (x * y);
                    }
                }
                // std::cerr << (view[y * width + x] ? "#" : ".");
            }
            // std::cerr << std::endl;
        }

        std::cout << resultTotal << std::endl;
    }

    void Part2()
    {
        auto state = Intcode::ParseProgram();
        // Switch to move mode
        state.program[0] = 2;

        std::vector<uint64_t> programInput{};
        std::string mainProgram = "A,B,B,C,B,C,B,C,A,A";
        std::string aProgram = "L,6,R,8,L,4,R,8,L,12";
        std::string bProgram = "L,12,R,10,L,4";
        std::string cProgram = "L,12,L,6,L,4,L,4";
        std::string liveFeed = "n";

        for (const auto& digit : mainProgram)
        {
            programInput.emplace_back((uint64_t)digit);
        }
        programInput.emplace_back(10);
        for (const auto& digit : aProgram)
        {
            programInput.emplace_back((uint64_t)digit);
        }
        programInput.emplace_back(10);
        for (const auto& digit : bProgram)
        {
            programInput.emplace_back((uint64_t)digit);
        }
        programInput.emplace_back(10);
        for (const auto& digit : cProgram)
        {
            programInput.emplace_back((uint64_t)digit);
        }
        programInput.emplace_back(10);
        for (const auto& digit : liveFeed)
        {
            programInput.emplace_back((uint64_t)digit);
        }
        programInput.emplace_back(10);

        const auto result = Intcode::ExecuteProgram(state, programInput);
        for (auto i : result)
        {
            std::cerr << (char)i;
        }
        std::cout << result[result.size() - 1] << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
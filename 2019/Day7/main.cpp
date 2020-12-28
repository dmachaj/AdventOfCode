#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "HelpfulInfrastructure.h"
#include "../Intcode/Intcode.h"

namespace
{
    void Part1()
    {
        auto originalState = Intcode::ParseProgram();

        uint32_t maxThrust{};
        std::array<uint32_t, 5> permutations{0, 1, 2, 3, 4};
        do
        {
            uint32_t thrust{};
            for (uint32_t i = 0; i < 5; ++i)
            {
                auto input = Intcode::VectorIntcodeInput({permutations[i], thrust});
                auto output = Intcode::VectorIntcodeOutput();
                auto programCopy = originalState;
                Intcode::ExecuteProgram(programCopy, &input, &output);
                thrust = (uint32_t)output.outputs[0];
            }

            if (thrust > maxThrust)
            {
                maxThrust = thrust;
            }
        }
        while (std::next_permutation(permutations.begin(), permutations.end()));
        
        std::cout << maxThrust << std::endl;
    }

    void Part2()
    {
        auto originalProgram = Intcode::ParseProgram();

        uint32_t maxThrust{};
        std::array<int64_t, 5> permutations{5, 6, 7, 8, 9};
        do
        {
            std::array<Intcode::ProgramState, 5> amplifiers{};
            for (auto& amplifier : amplifiers)
            {
                amplifier = originalProgram; // deep copy
            }

            uint32_t thrust{};
            bool allHalted{false};
            bool firstRun{true};
            while (!allHalted)
            {
                for (uint32_t i = 0; i < 5; ++i)
                {
                    std::vector<int64_t> currentInput{};
                    if (firstRun)
                    {
                        currentInput.emplace_back(permutations[i]);
                    }
                    currentInput.emplace_back(thrust);

                    Intcode::VectorIntcodeInput input(currentInput);
                    Intcode::VectorIntcodeOutput output;

                    ExecuteProgram(amplifiers[i], &input, &output);
                    thrust = (uint32_t)output.outputs[0];
                    if ((i == 4) && amplifiers[i].IsHalted())
                    {
                        allHalted = true;
                        break;
                    }
                    // if (i == 4) { std::cerr << "Current thrust is " << thrust << std::endl; }
                }
                firstRun = false;
            }

            if (thrust > maxThrust)
            {
                maxThrust = thrust;
            }
        }
        while (std::next_permutation(permutations.begin(), permutations.end()));
        
        std::cout << maxThrust << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
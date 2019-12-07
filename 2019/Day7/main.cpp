#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

namespace
{
    struct ProgramState
    {
        std::vector<int32_t> program{};
        uint32_t instructionCounter{};
    };

    std::pair<uint32_t, bool> ExecuteProgram(ProgramState& state, std::vector<uint32_t> inputs, bool returnOnOutput = false)
    {
        auto& program = state.program;
        auto& instructionCounter = state.instructionCounter;
        uint32_t resultCode{};
        uint32_t inputCount{};

        while (program[instructionCounter] != 99)
        {
            const auto opcode = program[instructionCounter] % 100;
            const bool firstParamImmediate = (program[instructionCounter] >= 100 ? (program[instructionCounter] / 100 % 10) : false);
            const bool secondParamImmediate = (program[instructionCounter] >= 1000 ? (program[instructionCounter] / 1000 % 10) : false);
            const bool thirdParamImmediate = (program[instructionCounter] >= 10000 ? (program[instructionCounter] / 10000 % 10) : false);

            switch (opcode)
            {
            case 1: // Add
            {
                const auto param1 = firstParamImmediate ? program[instructionCounter + 1] : program[program[instructionCounter + 1]];
                const auto param2 = secondParamImmediate ? program[instructionCounter + 2] : program[program[instructionCounter + 2]];
                const auto destination = program[instructionCounter + 3];
                program[destination] = param1 + param2;
                instructionCounter += 4;
                break;
            }
            case 2: // Multiply
            {
                const auto param1 = firstParamImmediate ? program[instructionCounter + 1] : program[program[instructionCounter + 1]];
                const auto param2 = secondParamImmediate ? program[instructionCounter + 2] : program[program[instructionCounter + 2]];
                const auto destination = program[instructionCounter + 3];
                program[destination] = param1 * param2;
                instructionCounter += 4;
                break;
            }
            case 3: // input param
            {
                const auto destination = program[instructionCounter + 1];
                program[destination] = inputs[inputCount++];
                instructionCounter += 2;
                break;
            }
            case 4: // output param
            {
                const auto param1 = firstParamImmediate ? program[instructionCounter + 1] : program[program[instructionCounter + 1]];
                resultCode = param1;
                // if (param1 == 0)
                // {
                //     // previous instruction passed self-test
                // }
                // else if (program[instructionCounter + 2] == 99)
                // {
                //     if (resultCode != 0)
                //     {
                //         std::cerr << "Error: Two output values" << std::end%l;
                //     }
                //     resultCode = param1;
                // }
                // else
                // {
                //     std::cerr << "Error" << std::endl;
                //     // error
                // }
                instructionCounter += 2;
                if (program[instructionCounter] != 99 && returnOnOutput)
                {
                    return std::make_pair(resultCode, false);
                }
                break;
            }
            case 5: // jump if true
            {
                const auto param1 = firstParamImmediate ? program[instructionCounter + 1] : program[program[instructionCounter + 1]];
                const auto param2 = secondParamImmediate ? program[instructionCounter + 2] : program[program[instructionCounter + 2]];
                if (param1 != 0)
                {
                    instructionCounter = param2;
                }
                else
                {
                    instructionCounter += 3;
                }
                break;
            }
            case 6: // jump if false
            {
                const auto param1 = firstParamImmediate ? program[instructionCounter + 1] : program[program[instructionCounter + 1]];
                const auto param2 = secondParamImmediate ? program[instructionCounter + 2] : program[program[instructionCounter + 2]];
                if (param1 == 0)
                {
                    instructionCounter = param2;
                }
                else
                {
                    instructionCounter += 3;
                }
                break;
            }
            case 7: // less than
            {
                const auto param1 = firstParamImmediate ? program[instructionCounter + 1] : program[program[instructionCounter + 1]];
                const auto param2 = secondParamImmediate ? program[instructionCounter + 2] : program[program[instructionCounter + 2]];
                const auto destination = program[instructionCounter + 3];
                program[destination] = (param1 < param2) ? 1 : 0;
                instructionCounter += 4;
                break;
            }
            case 8: // equals
            {
                const auto param1 = firstParamImmediate ? program[instructionCounter + 1] : program[program[instructionCounter + 1]];
                const auto param2 = secondParamImmediate ? program[instructionCounter + 2] : program[program[instructionCounter + 2]];
                const auto destination = program[instructionCounter + 3];
                program[destination] = (param1 == param2) ? 1 : 0;
                instructionCounter += 4;
                break;
            }
            default:
                std::cerr << "program[0] at time of crash = " << program[0] << std::endl;
                throw std::exception("Invalid opcode");
            }
        }

        const bool halted = (program[instructionCounter] == 99);
        if (halted && resultCode == 0)
        {
            resultCode = inputs[inputCount];
        }
        return std::make_pair(resultCode, halted);
    }
    
    void Part1()
    {
        std::vector<int32_t> program{};

        std::string input;
        while (std::getline(std::cin, input, ','))
        {
            program.emplace_back(std::atoi(input.c_str()));
        }

        uint32_t maxThrust{};
        std::array<uint32_t, 5> permutations{0, 1, 2, 3, 4};
        do
        {
            uint32_t thrust{};
            auto programCopy = program;
            for (uint32_t i = 0; i < 5; ++i)
            {
                ProgramState state {programCopy, 0};
                thrust = ExecuteProgram(state, {permutations[i], thrust}).first;
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
        std::vector<int32_t> program{};

        std::string input;
        while (std::getline(std::cin, input, ','))
        {
            program.emplace_back(std::atoi(input.c_str()));
        }

        uint32_t maxThrust{};
        std::array<uint32_t, 5> permutations{5, 6, 7, 8, 9};
        do
        {
            std::array<ProgramState, 5> amplifiers{};
            for (auto& amplifier : amplifiers)
            {
                amplifier = {program, 0}; // deep copy
            }

            uint32_t thrust{};
            bool allHalted{false};
            bool firstRun{true};
            while (!allHalted)
            {
                for (uint32_t i = 0; i < 5; ++i)
                {
                    std::vector<uint32_t> currentInput{};
                    if (firstRun)
                    {
                        currentInput.emplace_back(permutations[i]);
                    }
                    currentInput.emplace_back(thrust);

                    const auto result = ExecuteProgram(amplifiers[i], currentInput, true /* return on output */);
                    thrust = result.first;
                    if ((i == 4) && result.second)
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
    // Part1();
    Part2();
    return 0;
}
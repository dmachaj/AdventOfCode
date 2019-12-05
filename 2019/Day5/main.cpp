#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

namespace
{
    void ExecuteProgram(std::vector<int32_t>& program, uint32_t componentIdCode)
    {
        size_t instructionCounter{};
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
                program[destination] = componentIdCode;
                instructionCounter += 2;
                break;
            }
            case 4: // output param
            {
                const auto param1 = firstParamImmediate ? program[instructionCounter + 1] : program[program[instructionCounter + 1]];
                if (param1 == 0)
                {
                    // previous instruction passed self-test
                }
                else if (program[instructionCounter + 2] == 99)
                {
                    // final command before exit.  Diagnostic code is output
                    std::cout << "Diagnostic code: " << param1 << std::endl;
                }
                else
                {
                    std::cerr << "Error" << std::endl;
                    // error
                }
                instructionCounter += 2;
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
    }

    void Part1()
    {
        std::vector<int32_t> program{};

        std::string input;
        while (std::getline(std::cin, input, ','))
        {
            program.emplace_back(std::atoi(input.c_str()));
        }

        ExecuteProgram(program, 1 /* air conditioner */);
    }

    void Part2()
    {
        std::vector<int32_t> program{};

        std::string input;
        while (std::getline(std::cin, input, ','))
        {
            program.emplace_back(std::atoi(input.c_str()));
        }

        ExecuteProgram(program, 5 /* thermal thingy */);
    }
}

int main()
{
    // Part1();
    Part2();
    return 0;
}
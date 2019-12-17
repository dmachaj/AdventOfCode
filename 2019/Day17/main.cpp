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
        std::vector<int64_t> program{};
        uint64_t instructionCounter{};
        int64_t relativeBasePointer{};
    };

    bool ParamIsImmediate(int64_t data, uint64_t offset)
    {
        if ((uint64_t)data >= offset)
        {
            const auto result = (data / offset % 10);
            return (result == 1);
        } 
        return false;
    }

    bool ParamIsRelative(int64_t data, uint64_t offset)
    {
        if ((uint64_t)data >= offset)
        {
            const auto result = (data / offset % 10);
            return (result == 2);
        } 
        return false;
    }

    std::vector<int64_t> ExecuteProgram(ProgramState& state, std::vector<uint64_t> inputs/*, bool returnOnOutput = false*/)
    {
        auto& program = state.program;
        auto& instructionCounter = state.instructionCounter;
        std::vector<int64_t> result{};
        uint32_t inputCount{};

        while (program[instructionCounter] != 99)
        {
            const auto opcode = program[instructionCounter] % 100;
            const bool firstParamImmediate = ParamIsImmediate(program[instructionCounter], 100);
            const bool firstParamRelative = ParamIsRelative(program[instructionCounter], 100);
            const bool secondParamImmediate = ParamIsImmediate(program[instructionCounter], 1000);
            const bool secondParamRelative = ParamIsRelative(program[instructionCounter], 1000);
            const bool thirdParamImmediate = ParamIsImmediate(program[instructionCounter], 10000);
            const bool thirdParamRelative = ParamIsRelative(program[instructionCounter], 10000);

            switch (opcode)
            {
            case 1: // Add
            {
                const auto param1 = firstParamImmediate ? program[instructionCounter + 1] :
                                    firstParamRelative ? program[state.relativeBasePointer + program[instructionCounter + 1]] :
                                    program[program[instructionCounter + 1]];
                const auto param2 = secondParamImmediate ? program[instructionCounter + 2] :
                                    secondParamRelative ? program[state.relativeBasePointer + program[instructionCounter + 2]] :
                                    program[program[instructionCounter + 2]];
                const auto destination = thirdParamRelative ? state.relativeBasePointer + program[instructionCounter + 3] : program[instructionCounter + 3];
                program[destination] = param1 + param2;
                instructionCounter += 4;
                break;
            }
            case 2: // Multiply
            {
                const auto param1 = firstParamImmediate ? program[instructionCounter + 1] :
                                    firstParamRelative ? program[state.relativeBasePointer + program[instructionCounter + 1]] :
                                    program[program[instructionCounter + 1]];
                const auto param2 = secondParamImmediate ? program[instructionCounter + 2] :
                                    secondParamRelative ? program[state.relativeBasePointer + program[instructionCounter + 2]] :
                                    program[program[instructionCounter + 2]];
                const auto destination = thirdParamRelative ? state.relativeBasePointer + program[instructionCounter + 3] : program[instructionCounter + 3];
                program[destination] = param1 * param2;
                instructionCounter += 4;
                break;
            }
            case 3: // input param
            {
                const auto destination = firstParamRelative ? state.relativeBasePointer : program[instructionCounter + 1];
                program[destination] = inputs[inputCount++];
                instructionCounter += 2;
                break;
            }
            case 4: // output param
            {
                const auto param1 = firstParamImmediate ? program[instructionCounter + 1] :
                                    firstParamRelative ? program[state.relativeBasePointer + program[instructionCounter + 1]] :
                                    program[program[instructionCounter + 1]];
                result.emplace_back(param1);
                instructionCounter += 2;
                // Guess - return when second output is reached
                // if (result.size() == 2) { return result; }
                break;
            }
            case 5: // jump if true
            {
                const auto param1 = firstParamImmediate ? program[instructionCounter + 1] :
                                    firstParamRelative ? program[state.relativeBasePointer + program[instructionCounter + 1]] :
                                    program[program[instructionCounter + 1]];
                const auto param2 = secondParamImmediate ? program[instructionCounter + 2] :
                                    secondParamRelative ? program[state.relativeBasePointer + program[instructionCounter + 2]] :
                                    program[program[instructionCounter + 2]];
                if (param1 != 0)
                {
                    instructionCounter = (uint64_t)param2;
                }
                else
                {
                    instructionCounter += 3;
                }
                break;
            }
            case 6: // jump if false
            {
                const auto param1 = firstParamImmediate ? program[instructionCounter + 1] :
                                    firstParamRelative ? program[state.relativeBasePointer + program[instructionCounter + 1]] :
                                    program[program[instructionCounter + 1]];
                const auto param2 = secondParamImmediate ? program[instructionCounter + 2] :
                                    secondParamRelative ? program[state.relativeBasePointer + program[instructionCounter + 2]] :
                                    program[program[instructionCounter + 2]];
                if (param1 == 0)
                {
                    instructionCounter = (uint64_t)param2;
                }
                else
                {
                    instructionCounter += 3;
                }
                break;
            }
            case 7: // less than
            {
                const auto param1 = firstParamImmediate ? program[instructionCounter + 1] :
                                    firstParamRelative ? program[state.relativeBasePointer + program[instructionCounter + 1]] :
                                    program[program[instructionCounter + 1]];
                const auto param2 = secondParamImmediate ? program[instructionCounter + 2] :
                                    secondParamRelative ? program[state.relativeBasePointer + program[instructionCounter + 2]] :
                                    program[program[instructionCounter + 2]];
                const auto destination = thirdParamRelative ? state.relativeBasePointer + program[instructionCounter + 3] : program[instructionCounter + 3];
                program[destination] = (param1 < param2) ? 1 : 0;
                instructionCounter += 4;
                break;
            }
            case 8: // equals
            {
                const auto param1 = firstParamImmediate ? program[instructionCounter + 1] :
                                    firstParamRelative ? program[state.relativeBasePointer + program[instructionCounter + 1]] :
                                    program[program[instructionCounter + 1]];
                const auto param2 = secondParamImmediate ? program[instructionCounter + 2] :
                                    secondParamRelative ? program[state.relativeBasePointer + program[instructionCounter + 2]] :
                                    program[program[instructionCounter + 2]];
                const auto destination = thirdParamRelative ? state.relativeBasePointer + program[instructionCounter + 3] : program[instructionCounter + 3];
                program[destination] = (param1 == param2) ? 1 : 0;
                instructionCounter += 4;
                break;
            }
            case 9: // relative base pointer adjustment
            {
                const auto param1 = firstParamImmediate ? program[instructionCounter + 1] :
                                    firstParamRelative ? program[state.relativeBasePointer + program[instructionCounter + 1]] :
                                    program[program[instructionCounter + 1]];
                state.relativeBasePointer += param1;
                instructionCounter += 2;
                break;
            }
            default:
                std::cerr << "program[0] at time of crash = " << program[0] << std::endl;
                throw std::exception("Invalid opcode");
            }
        }
        return result;
    }

    constexpr size_t c_memorySize{10000};
    void Part1()
    {
        std::vector<int64_t> program{};
        program.reserve(c_memorySize);

        std::string input;
        while (std::getline(std::cin, input, ','))
        {
            program.emplace_back(std::atoll(input.c_str()));
        }

        for (auto i = program.size(); i < c_memorySize; ++i)
        {
            program.emplace_back(0);
        }

        ProgramState state {program, 0, 0};
        std::vector<uint64_t> programInput{0ULL};
        const auto result = ExecuteProgram(state, programInput);

        std::vector<bool> view{};
        uint32_t width{};
        uint32_t counter{};
        for (const auto& letter : result)
        {
            if (letter == '#')
            {
                std::cerr << "#";
            }
            else if (letter == '.')
            {
                std::cerr << ".";
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
            std::cerr << std::endl;
        }

        std::cout << resultTotal << std::endl;
    }

    void Part2()
    {
        std::cout << 0 << std::endl;
    }
}

int main()
{
    Part1();
    // Part2();
    return 0;
}
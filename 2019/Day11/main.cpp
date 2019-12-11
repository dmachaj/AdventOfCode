#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_set>

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
                if (result.size() == 2) { return result; }
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

        // const bool halted = (program[instructionCounter] == 99);
        // if (halted && (resultCode == 0) && (inputCount < inputs.size()))
        // {
        //     resultCode = inputs[inputCount];
        // }
        // return std::make_pair(resultCode, halted);
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

        constexpr size_t c_width{100};
        constexpr size_t c_height{100};
        std::array<bool, c_width * c_height> panel{false}; // false = black, true = white
        std::array<bool, c_width * c_height> visitedPanels{false};
        std::pair<uint32_t, uint32_t> robotPosition{50, 50};
        int32_t robotDirection{0}; // 0 = up, 1 = right, 2 = down, 3 = left

        ProgramState state {program, 0, 0};
        while (true)
        {
            const auto currentIndex = (robotPosition.second * c_width) + robotPosition.first;
            const auto currentColor = panel[currentIndex];
            std::vector<uint64_t> programInput{currentColor ? 1ULL : 0ULL};
            const auto result = ExecuteProgram(state, programInput);
            if (result.size() == 0)
            {
                break;
            }

            // std::cerr << "Index: " << currentIndex << std::endl;
            // std::cerr << "Robot at [" << robotPosition.first << "][" << robotPosition.second << "].  Will paint " << 
            //     (result[0] ? "white" : "black") << " and then turn " <<
            //     (result[1] ? "right" : "left") << std::endl;

            if (result[0] == 1ULL)
            {
                panel[currentIndex] = true;
            }
            else
            {
                panel[currentIndex] = false;
            }
            
            visitedPanels[currentIndex] = true;

            if (result[1] == 1) // turn right
            {
                robotDirection++;
                robotDirection %= 4;
            }
            else
            {
                robotDirection--;
                if (robotDirection < 0)
                {
                    robotDirection = 3;
                }
            }
            
            switch (robotDirection)
            {
                case 0: // up
                    robotPosition.second--;
                    break;
                case 1: // right
                    robotPosition.first++;
                    break;
                case 2: // down
                    robotPosition.second++;
                    break;
                case 3: // left
                    robotPosition.first--;
                    break;
                default:
                    throw std::exception("oops");
            }
        }

        uint32_t visitedPositions{};
        for (const auto cell : visitedPanels)
        {
            if (cell)
            {
                visitedPositions++;
            }
        }
        std::cout << visitedPositions << std::endl;
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
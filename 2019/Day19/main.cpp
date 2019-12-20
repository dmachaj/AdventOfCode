#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

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
                const auto destination = firstParamRelative ? state.relativeBasePointer + program[instructionCounter + 1] : program[instructionCounter + 1];
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

        constexpr auto c_width = 50;
        constexpr auto c_height = 50;
        std::array<bool, c_width * c_height> results{};

        for (auto y = 0UL; y < c_height; ++y)
        {
            bool anyXMatches{false};
            for (auto x = 0UL; x < c_width; ++x)
            {
                ProgramState state {program, 0, 0};
                std::vector<uint64_t> programInput{x, y};
                const auto result = ExecuteProgram(state, programInput);
                if ((result.size() > 0) && (result[0] != 0))
                {
                    results[y * c_width + x] = true;
                    anyXMatches = true;
                }
                else if ((results.size() > 0) && (result[0] == 0) && anyXMatches)
                {
                    break;
                }
            }
        }

        // for (auto y = 0UL; y < c_height; ++y)
        // {
        //     for (auto x = 0UL; x < c_width; ++x)
        //     {
        //         std::cerr << (results[y * c_width + x] ? "#" : ".");
        //     }
        //     std::cerr << std::endl;
        // }

        uint32_t affectedSquares{};
        for (const auto square : results)
        {
            if (square) { affectedSquares++; }
        }
        std::cout << affectedSquares << std::endl;
    }

    static std::unordered_map<uint32_t, bool> resultCache{};
    bool IsSquareAffected(const std::vector<int64_t>& program, const uint32_t x, const uint32_t y)
    {
        const auto cache = resultCache.find((x * 10000) + y);
        if (cache != resultCache.end())
        {
            return cache->second;
        }

        ProgramState state {program, 0, 0};
        std::vector<uint64_t> programInput{x, y};
        const auto intCodeResult = ExecuteProgram(state, programInput);
        const bool result = ((intCodeResult.size() != 0) && (intCodeResult[0] != 0));
        resultCache.emplace((x * 10000) + y, result);
        return result;
    }

    uint32_t c_width = 10000;
    uint32_t c_height = 10000;
    constexpr uint32_t necessaryWidth{ 100 };
    constexpr uint32_t necessaryHeight{ 100 };
    bool DoesSquareFitWithin(const std::vector<int64_t>& program, const uint32_t x, const uint32_t y)
    {
        for (auto iy = y; iy < (y + necessaryHeight); ++iy)
        {
            for (auto ix = x; ix < (x + necessaryWidth); ++ix)
            {
                if (!IsSquareAffected(program, ix, iy))
                {
                    // std::cerr << "Square starting at (" << x << "," << y << ") failed at " << ((iy - y != 0) ? necessaryWidth : (ix - x)) << " wide and " << (iy - y) << " tall." << std::endl;
                    return false;
                }
            }
        }
        return true;
    }

    void Part2()
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

        uint32_t minX = 0UL;
        for (auto y = 0UL; y < c_height; ++y)
        {
            for (auto x = minX; x < c_width; ++x)
            {
                if (IsSquareAffected(program, x, y))
                {
                    if ((x - 1 > 0) && !IsSquareAffected(program, x - 1, y))
                    {
                        minX = x;
                    }

                    if (DoesSquareFitWithin(program, x, y))
                    {
                        const auto finalResult = (x * 10000) + y;
                        std::cout << finalResult << std::endl;
                        return;
                    }
                    else if (!IsSquareAffected(program, x + 1, y))
                    {
                        break;
                    }
                }
            }
        }

        std::cerr << "Did not find a match :(" << std::endl;
        std::cout << 0 << std::endl;
    }
}

int main()
{
    // Part1();
    Part2();
    return 0;
}
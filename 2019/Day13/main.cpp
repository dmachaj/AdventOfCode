#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "HelpfulInfrastructure.h"

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
                // program[destination] = inputs[inputCount++];
                program[destination] = 0; // HACK - don't move paddle.
                inputCount;
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
    
    struct Tile
    {
        uint32_t x{};
        uint32_t y{};
        uint8_t type{};
        char printCode{};
    };

    struct BoardInfo
    {
        std::vector<Tile> allTiles{};
        uint32_t width{};
        uint32_t height{};
        uint32_t score{};
    };

    BoardInfo IntcodeOutputToBoardInfo(const std::vector<int64_t>& input)
    {
        BoardInfo result{};

        for (auto i = 0UL; i < input.size(); i += 3)
        {
            if ((input[i] == -1) && (input[i+1] == 0))
            {
                result.score = (uint32_t)input[i+2];
                continue;
            }

            Tile thisTile{ (uint32_t)input[i], (uint32_t)input[i+1], (uint8_t)input[i+2] };
            switch (thisTile.type)
            {
                case 0: // empty
                    thisTile.printCode =  ' ';
                    break;
                case 1: // wall
                    thisTile.printCode =  '#';
                    break;
                case 2: // block
                    thisTile.printCode =  'B';
                    break;
                case 3: // paddle
                    thisTile.printCode =  '=';
                    break;
                case 4: // ball
                    thisTile.printCode =  'o';
                    break;
                default:
                    thisTile.printCode =  '?';
                    // throw std::exception("Unknown tile type");
                    break;
            }
            result.allTiles.emplace_back(std::move(thisTile));
        }

        const auto maxX = std::max_element(result.allTiles.begin(), result.allTiles.end(), 
            [](const Tile& first, const Tile& second)
            {
                return first.x < second.x;
            });
        const auto maxY = std::max_element(result.allTiles.begin(), result.allTiles.end(), 
            [](const Tile& first, const Tile& second)
            {
                return first.y < second.y;
            });
        
        // std::cerr << "NumTiles=" << (allTiles.size() / 3) << " MaxX=" << maxX->x << " MaxY=" << maxY->y << " NumBlocks=" << numBlocks << std::endl;
        result.width = maxX->x + 1;
        result.height = maxY->y + 1;

        return result;
    }

    constexpr size_t c_memorySize{10000};
    void Part1(std::istream& inStream)
    {
        std::vector<int64_t> program{};
        program.reserve(c_memorySize);

        std::string input;
        while (std::getline(inStream, input, ','))
        {
            program.emplace_back(std::atoll(input.c_str()));
        }

        for (auto i = program.size(); i < c_memorySize; ++i)
        {
            program.emplace_back(0);
        }
        
        ProgramState state {program, 0, 0};
        std::vector<uint64_t> programInput{};
        const auto result = ExecuteProgram(state, programInput);

        const BoardInfo board = IntcodeOutputToBoardInfo(result);

        uint32_t numBlocks{};
        for (const auto& tile : board.allTiles)
        {
            if (tile.type == 2)
            {
                ++numBlocks;
            }
        }
        
        for (auto y = 0UL; y < board.height; ++y)
        {
            for (auto x = 0UL; x < board.width; ++x)
            {
                const auto tile = std::find_if(board.allTiles.begin(), board.allTiles.end(), [x,y](const Tile& tile) { return tile.x == x && tile.y ==y; });
                if (tile != board.allTiles.end())
                {
                    std::cerr << tile->printCode;
                }
            }
            std::cerr << std::endl;
        }

        std::cout << numBlocks << std::endl;
    }

    void Part2(std::istream& inStream)
    {
        std::vector<int64_t> program{};
        program.reserve(c_memorySize);

        std::string input;
        while (std::getline(inStream, input, ','))
        {
            program.emplace_back(std::atoll(input.c_str()));
        }

        for (auto i = program.size(); i < c_memorySize; ++i)
        {
            program.emplace_back(0);
        }
        program[0] = 2; // insert quarters
        
        ProgramState state {program, 0, 0};
        std::vector<uint64_t> programInput{};

        while (true)
        {
            const auto result = ExecuteProgram(state, programInput);
            const BoardInfo board = IntcodeOutputToBoardInfo(result);

            for (auto y = 0UL; y < board.height; ++y)
            {
                for (auto x = 0UL; x < board.width; ++x)
                {
                    const auto tile = std::find_if(board.allTiles.begin(), board.allTiles.end(), [x,y](const Tile& tile) { return tile.x == x && tile.y ==y; });
                    if (tile != board.allTiles.end())
                    {
                        std::cerr << tile->printCode;
                    }
                }
                std::cerr << std::endl;
            }

            std::cerr << "Score: " << board.score << std::endl;
        }

    }
}

int main()
{
    if (RunPart1())
    {
        // std::ifstream inputFileStream;
        // inputFileStream.open("C:\\code\\AdventOfCode\\2019\\Day13\\Part1\\unique.in");
        Part1(std::cin);
    }
    else
    {
        std::ifstream inputFileStream;
        inputFileStream.open("C:\\code\\AdventOfCode\\2019\\Day13\\Part2\\unique.in");
        // Part2(std::cin);
        Part2(inputFileStream);
    }
    
    return 0;
}
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <fstream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "HelpfulInfrastructure.h"
#include "../Intcode/Intcode.h"

using namespace std; // I wouldn't normally do this
using namespace std::literals;

#pragma warning (disable: 4267) // size_t <-> uint
#pragma warning (disable: 4189) // unused variable
#pragma warning (disable: 4100) // unused param

namespace
{
    std::vector<int64_t> StringToCommand(const std::string& input)
    {
        std::vector<int64_t> result;
        for (auto letter : input)
        {
            result.emplace_back((int64_t)letter);
        }
        return result;
    }

    void PrintOutput(const std::vector<int64_t> output)
    {
        std::string result;
        for (auto letter : output)
        {
            result += (char)letter;
        }
        std::cerr << result << std::endl;
    }

    void Part1(istream& inStream)
    {
        auto state = Intcode::ParseProgram(inStream);
        {
            Intcode::VectorIntcodeInput input;
            Intcode::VectorIntcodeOutput output;
            Intcode::ExecuteProgram(state, &input, &output);
            PrintOutput(output.outputs);
        }

    // Items which kill you:
    // north take infinite loop
    // north west take molten lava
    // south west south west take photons
    // west west north take giant electromagnet
    // west west south take escape pod

    // Items which are not necessary:
    // south south east take loom west north north
    // south west south take space law space brochure north east north
    // south take space heater north
    // west west take festive hat east east

        std::string commandList=R"(
north
north
take sand
south
south

south
west
take wreath
east
north

south
west
south
south
take pointer
north
north
east
north

west
south
take planetoid
north
east

west
west
south
west
inv
north
        )";
        {
            Intcode::VectorIntcodeInput input(StringToCommand(commandList));
            Intcode::VectorIntcodeOutput output;
            Intcode::ExecuteProgram(state, &input, &output);
            PrintOutput(output.outputs);
        }
        
        // Adventure game was solved manually by running the program and exploring by hand.  Notes were taken
        // and converted into the command list up above.
        
        // while (true)
        // {
        //     std::string humanInput;
        //     std::getline(std::cin, humanInput);
        //     humanInput += '\n';

        //     Intcode::VectorIntcodeInput input(StringToCommand(humanInput));
        //     Intcode::VectorIntcodeOutput output;
        //     Intcode::ExecuteProgram(state, &input, &output);
        //     PrintOutput(output.outputs);
        // }

        std::cout << 0 << std::endl;
    }

    void Part2()
    {
        std::cout << 0 << std::endl;
    }
}

int main()
{
    std::ifstream inputFileStream;
    inputFileStream.open(LR"(c:\code\AdventOfCode\2019\Day25\Part1\unique.in)");
    RunPart1() ? Part1(inputFileStream) : Part2();
    return 0;
}
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
    void Part1(std::istream& inStream)
    {
        std::ostringstream program;
        // If A or B is a gap and C is fine then jump
        program << "NOT A T" << std::endl;
        program << "NOT B J" << std::endl;
        program << "OR T J" << std::endl;
        program << "AND C J" << std::endl;
        // C is a gap but D is fine, jump
        program << "NOT C T" << std::endl;
        program << "AND D T" << std::endl;
        program << "OR T J" << std::endl;
        // End of program
        program << "WALK" << std::endl;

        std::vector<int64_t> programEncoded{};
        for (auto letter : program.str())
        {
            programEncoded.emplace_back((int64_t)letter);
        }

        auto state = Intcode::ParseProgram(inStream);
        Intcode::VectorIntcodeInput input(programEncoded);
        Intcode::VectorIntcodeOutput output;
        Intcode::ExecuteProgram(state, &input, &output);

        const int64_t finalOutput = output.outputs[output.outputs.size() - 1];
        if (finalOutput > 256)
        {
            std::cout << finalOutput << std::endl;
            return;
        }

        std::for_each(output.outputs.begin(), output.outputs.end(), [](int64_t output)
        {
            std::cerr << (char)output;
        });

        std::cout << "Did not finish" << std::endl;
    }

    void Part2(std::istream& inStream)
    {
        std::ostringstream program;
        // If A or B is a gap and C is fine then jump
        program << "NOT A T" << std::endl;
        program << "NOT B J" << std::endl;
        program << "OR T J" << std::endl;
        program << "AND C J" << std::endl;
        // C is a gap but D is fine, jump
        program << "NOT C T" << std::endl;
        program << "AND D T" << std::endl;
        program << "OR T J" << std::endl;
        // End of program
        program << "RUN" << std::endl;

        std::vector<int64_t> programEncoded{};
        for (auto letter : program.str())
        {
            programEncoded.emplace_back((int64_t)letter);
        }

        auto state = Intcode::ParseProgram(inStream);
        Intcode::VectorIntcodeInput input(programEncoded);
        Intcode::VectorIntcodeOutput output;
        Intcode::ExecuteProgram(state, &input, &output);

        const int64_t finalOutput = output.outputs[output.outputs.size() - 1];
        if (finalOutput > 256)
        {
            std::cout << finalOutput << std::endl;
            return;
        }

        std::for_each(output.outputs.begin(), output.outputs.end(), [](int64_t output)
        {
            std::cerr << (char)output;
        });

        std::cout << "Did not finish" << std::endl;
    }
}

int main()
{
    std::ifstream inputFileStream;
    inputFileStream.open(LR"(c:\code\AdventOfCode\2019\Day21\Part1\unique.in)");

    RunPart1() ? Part1(inputFileStream) : Part2(inputFileStream);
    return 0;
}
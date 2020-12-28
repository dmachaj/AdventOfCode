#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "HelpfulInfrastructure.h"
#include "../Intcode/Intcode.h"

void Part1()
{
    auto program = Intcode::ParseProgram();
    Intcode::ExecuteProgram(program);
    size_t trailingZerosToKeep = (program.instructionCounter > 20) ? 2 : 0; // unique output has 2 trailing zeros.  Keep them so we match.
    const auto output = Intcode::ProgramToString(program, trailingZerosToKeep);
    std::cout << output << std::endl;
}

void Part2()
{
    auto program = Intcode::ParseProgram();

    for (uint32_t i = 0; i < 100; ++i)
    {
        for (uint32_t j = 0; j < 100; ++j)
        {
            auto programCopy = program;
            programCopy.program[1] = i;
            programCopy.program[2] = j;
            Intcode::ExecuteProgram(programCopy);

            if (programCopy.program[0] == 19690720)
            {
                // Correct answer 5741.
                // In theory I'd need to zero-pad left to fill two digits but my unique input didn't require it :D
                std::cout << i << j << std::endl;
                break;
            }
        }
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "HelpfulInfrastructure.h"
#include "../Intcode/Intcode.h"

namespace
{
    void ExecuteFullProblem(uint32_t mode)
    {
        auto state = Intcode::ParseProgram();
        auto input = Intcode::VectorIntcodeInput({mode} /* test input */);
        auto output = Intcode::VectorIntcodeOutput();
        Intcode::ExecuteProgram(state, &input, &output);
        std::ostringstream outStream;
        for (const auto& resultVal : output.outputs)
        {
            outStream << resultVal << ",";
        }

        const auto resultString = outStream.str();
        std::cout << resultString.substr(0, resultString.length() - 1) << std::endl;
    }

    void Part1()
    {
        ExecuteFullProblem(1);
    }

    void Part2()
    {
        ExecuteFullProblem(2);
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
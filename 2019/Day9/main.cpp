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
        const auto result = Intcode::ExecuteProgram(state, {mode} /* test input */);
        std::ostringstream outStream;
        for (const auto& resultVal : result )
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
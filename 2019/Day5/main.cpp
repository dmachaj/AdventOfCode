#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "HelpfulInfrastructure.h"
#include "../Intcode/Intcode.h"

namespace
{
    void Part1()
    {
        auto program = Intcode::ParseProgram();
        const auto result = Intcode::ExecuteProgram(program, {1} /* air conditioner */);
        const auto succeeded = std::all_of(result.begin(), --result.end(), [](const auto result) { return result == 0; });
        if (succeeded)
        {
            std::cout << result[result.size() - 1] << std::endl;
        }
        else
        {
            std::cout << "Diagnostic code: " << result[1] << std::endl;
        }
    }

    void Part2()
    {
        auto program = Intcode::ParseProgram();
        const auto result = Intcode::ExecuteProgram(program, {5} /* thermal thingy */);
        const auto succeeded = std::all_of(result.begin(), --result.end(), [](const auto result) { return result == 0; });
        if (succeeded)
        {
            std::cout << result[result.size() - 1] << std::endl;
        }
        else
        {
            std::cout << "Diagnostic code: " << result[1] << std::endl;
        }
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
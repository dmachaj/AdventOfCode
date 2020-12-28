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
        auto input = Intcode::VectorIntcodeInput({1} /* air conditioner */);
        auto output = Intcode::VectorIntcodeOutput();
        Intcode::ExecuteProgram(program, &input, &output);
        const auto succeeded = std::all_of(output.outputs.begin(), --output.outputs.end(), [](const auto result) { return result == 0; });
        if (succeeded)
        {
            std::cout << output.outputs[output.outputs.size() - 1] << std::endl;
        }
        else
        {
            std::cout << "Diagnostic code: " << output.outputs[1] << std::endl;
        }
    }

    void Part2()
    {
        auto program = Intcode::ParseProgram();
        auto input = Intcode::VectorIntcodeInput({5} /* thermal thingy */);
        auto output = Intcode::VectorIntcodeOutput();
        Intcode::ExecuteProgram(program, &input, &output);
        const auto succeeded = std::all_of(output.outputs.begin(), --output.outputs.end(), [](const auto result) { return result == 0; });
        if (succeeded)
        {
            std::cout << output.outputs[output.outputs.size() - 1] << std::endl;
        }
        else
        {
            std::cout << "Diagnostic code: " << output.outputs[1] << std::endl;
        }
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
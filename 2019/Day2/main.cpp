#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

void part1()
{
    std::vector<int32_t> program{};

    std::string input;
    while (std::getline(std::cin, input, ','))
    {
        program.emplace_back(std::atoi(input.c_str()));
    }

    size_t instructionCounter{};
    while (program[instructionCounter] != 99)
    {
        const auto param1 = program[program[instructionCounter + 1]];
        const auto param2 = program[program[instructionCounter + 2]];
        const auto destination = program[instructionCounter + 3];

        switch (program[instructionCounter])
        {
        case 1:
            program[destination] = param1 + param2;
            break;
        case 2:
            program[destination] = param1 * param2;
            break;
        default:
            std::cerr << "program[0] at time of crash = " << program[0] << std::endl;
            throw std::exception("Invalid opcode");
        }

        instructionCounter += 4;
    }

    std::ostringstream outputStream{};
    for (int i = 0; i < program.size(); ++i)
    {
        outputStream << program[i] << ",";
    }
    std::string output = outputStream.str();
    output.erase(output.length() - 1, 1); // remove trailing comma
    std::cout << output << std::endl;
}

int main()
{
    part1();
    return 0;
}
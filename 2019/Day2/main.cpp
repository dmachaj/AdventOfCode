#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

// The following is a copy/paste of my unique program input.
constexpr std::string_view c_uniqueInput{ "1,12,2,3,1,1,2,3,1,3,4,3,1,5,0,3,2,6,1,19,1,5,19,23,1,13,23,27,1,6,27,31,2,31,13,35,1,9,35,39,2,39,13,43,1,43,10,47,1,47,13,51,2,13,51,55,1,55,9,59,1,59,5,63,1,6,63,67,1,13,67,71,2,71,10,75,1,6,75,79,1,79,10,83,1,5,83,87,2,10,87,91,1,6,91,95,1,9,95,99,1,99,9,103,2,103,10,107,1,5,107,111,1,9,111,115,2,13,115,119,1,119,10,123,1,123,10,127,2,127,10,131,1,5,131,135,1,10,135,139,1,139,2,143,1,6,143,0,99,2,14,0,0"};

void ExecuteProgram(std::vector<int32_t>& program)
{
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
}

void Part1()
{
    std::vector<int32_t> program{};

    std::string input;
    while (std::getline(std::cin, input, ','))
    {
        program.emplace_back(std::atoi(input.c_str()));
    }

    ExecuteProgram(program);

    std::ostringstream outputStream{};
    for (int i = 0; i < program.size(); ++i)
    {
        outputStream << program[i] << ",";
    }
    std::string output = outputStream.str();
    output.erase(output.length() - 1, 1); // remove trailing comma
    std::cout << output << std::endl;
}

void Part2()
{
    std::vector<int32_t> program{};

    std::string input;
    std::istringstream inStream(c_uniqueInput.data());
    while (std::getline(inStream, input, ','))
    {
        program.emplace_back(std::atoi(input.c_str()));
    }

    for (uint32_t i = 0; i < 100; ++i)
    {
        for (uint32_t j = 0; j < 100; ++j)
        {
            auto programCopy = program;
            programCopy[1] = i;
            programCopy[2] = j;
            ExecuteProgram(programCopy);

            if (programCopy[0] == 19690720)
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
    // Part1();
    Part2();
    return 0;
}
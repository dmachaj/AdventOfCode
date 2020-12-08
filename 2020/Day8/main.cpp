#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <set>
#include <string>
#include <sstream>
#include <vector>
#include "HelpfulInfrastructure.h"

using namespace std; // I wouldn't normally do this
using namespace std::literals;

#pragma warning (disable: 4267) // size_t <-> uint

namespace
{
    enum class InstructionType
    {
        NOP,
        ACC,
        JMP
    };

    struct Instruction
    {
        InstructionType type{};
        int32_t arg{};
    };

    void Part1()
    {
        vector<Instruction> instructions;

        std::string input;
        while (std::getline(std::cin, input))
        {
            InstructionType type{};
            if (input.find("nop") != string::npos)
            {
                type = InstructionType::NOP;
            }
            else if (input.find("acc") != string::npos)
            {
                type = InstructionType::ACC;
            }
            else if (input.find("jmp") != string::npos)
            {
                type = InstructionType::JMP;
            }
            else
            {
                throw 0;
            }

            istringstream instream(input);
            string temp;
            int32_t argument;
            instream >> temp >> argument;

            instructions.emplace_back(Instruction{type, argument});
        }

        set<uint32_t> instructionsReached{};
        uint32_t programCounter{};
        int32_t accumulator{};

        while(find(instructionsReached.begin(), instructionsReached.end(), programCounter) == instructionsReached.end())
        {
            instructionsReached.emplace(programCounter);
            
            switch (instructions[programCounter].type)
            {
                case InstructionType::NOP:
                    programCounter++;
                    break;
                case InstructionType::ACC:
                    accumulator += instructions[programCounter].arg;
                    programCounter++;
                    break;
                case InstructionType::JMP:
                    programCounter += instructions[programCounter].arg;
                    break;
            }
        }

        std::cout << accumulator << std::endl;
    }

    void Part2()
    {
        vector<Instruction> instructions;

        std::string input;
        while (std::getline(std::cin, input))
        {
            InstructionType type{};
            if (input.find("nop") != string::npos)
            {
                type = InstructionType::NOP;
            }
            else if (input.find("acc") != string::npos)
            {
                type = InstructionType::ACC;
            }
            else if (input.find("jmp") != string::npos)
            {
                type = InstructionType::JMP;
            }
            else
            {
                throw 0;
            }

            istringstream instream(input);
            string temp;
            int32_t argument;
            instream >> temp >> argument;

            instructions.emplace_back(Instruction{type, argument});
        }

        const auto programTerminates = [](const auto& instructions) -> pair<bool, int32_t>
        {
            set<uint32_t> instructionsReached{};
            uint32_t programCounter{};
            int32_t accumulator{};
            bool halted{false};

            while(find(instructionsReached.begin(), instructionsReached.end(), programCounter) == instructionsReached.end())
            {
                instructionsReached.emplace(programCounter);
                
                switch (instructions[programCounter].type)
                {
                    case InstructionType::NOP:
                        programCounter++;
                        break;
                    case InstructionType::ACC:
                        accumulator += instructions[programCounter].arg;
                        programCounter++;
                        break;
                    case InstructionType::JMP:
                        programCounter += instructions[programCounter].arg;
                        break;
                }

                if (programCounter == instructions.size())
                {
                    halted = true;
                    break;
                }
            }

            return make_pair(halted, accumulator);
        };

        for (auto i = 0; i < instructions.size(); ++i)
        {
            if (instructions[i].type == InstructionType::ACC) continue;
            auto instructionsCopy = instructions;
            instructionsCopy[i].type = (instructionsCopy[i].type == InstructionType::NOP) ? InstructionType::JMP : InstructionType::NOP;
            const auto result = programTerminates(instructionsCopy);
            if (result.first)
            {
                std::cout << result.second << endl;
                return;
            }
        }

        std::cout << "NAN" << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
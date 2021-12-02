#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "HelpfulInfrastructure.h"

using namespace std; // I wouldn't normally do this
using namespace std::literals;

#pragma warning (disable: 4267) // size_t <-> uint
#pragma warning (disable: 4189) // unused variable
#pragma warning (disable: 4100) // unused param

namespace
{
    void Part1()
    {
        std::vector<std::string> commands;
        {
            std::string input;
            while (std::getline(std::cin, input))
            {
                commands.emplace_back(move(input));
            }
        }

        uint32_t depth{};
        uint32_t x{};
        for (const auto& command : commands)
        {
            uint32_t value = std::atoi(command.substr(command.find(' ') + 1).c_str());
            if (command[0] == 'f')
            {
                x += value;
            }
            else if (command[0] == 'd')
            {
                depth += value;
            }
            else if (command[0] == 'u')
            {
                depth -= value;
            }
        }

        std::cout << (depth * x) << std::endl;
    }

    void Part2()
    {
        std::vector<std::string> commands;
        {
            std::string input;
            while (std::getline(std::cin, input))
            {
                commands.emplace_back(move(input));
            }
        }

        uint32_t depth{};
        uint32_t x{};
        uint32_t aim{};
        for (const auto& command : commands)
        {
            uint32_t value = std::atoi(command.substr(command.find(' ') + 1).c_str());
            if (command[0] == 'f')
            {
                depth += (value * aim);
                x += value;
            }
            else if (command[0] == 'd')
            {
                aim += value;
            }
            else if (command[0] == 'u')
            {
                aim -= value;
            }
        }

        std::cout << (depth * x) << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
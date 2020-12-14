#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include "HelpfulInfrastructure.h"

using namespace std; // I wouldn't normally do this
using namespace std::literals;

#pragma warning (disable: 4267) // size_t <-> uint
#pragma warning (disable: 4189) // unused variable

namespace
{
    std::pair<uint64_t, uint64_t> ParseMask(const std::string& input)
    {
        auto maskStr = input.substr(7);

        uint64_t zeroMask{};
        for (auto i = input.size(); i > 0; --i)
        {
            char bit = input[i - 1];
            if (bit == '0')
            {
                zeroMask |= (1ULL << (input.size() - i));
            }
        }

        uint64_t oneMask = {};
        for (auto i = input.size(); i > 0; --i)
        {
            char bit = input[i - 1];
            if (bit == '1')
            {
                oneMask |= (1ULL << (input.size() - i));
            }
        }

        return make_pair(zeroMask, oneMask);
    }

    void Part1()
    {
        std::vector<string> commands{};
        std::string input;
        while (std::getline(std::cin, input))
        {
            commands.emplace_back(move(input));
        }

        // address, value
        map<uint64_t, uint64_t> memory{};

        uint64_t zeroMask{};
        uint64_t oneMask{};
        for (const auto& command : commands)
        {
            if (command[1] == 'a')
            {
                auto masks = ParseMask(command);
                zeroMask = masks.first;
                oneMask = masks.second;
                continue;
            }

            const auto openingBracket = command.find('[');
            const auto closingBracket = command.find(']');
            uint64_t address = std::atoll(command.substr(openingBracket + 1, closingBracket - openingBracket - 1).c_str());
            uint64_t value = std::atoi(command.substr(command.find('=') + 2).c_str());

            uint64_t result{value};
            result = result | oneMask;
            result = result & (~zeroMask);

            memory[address] = result;
        }

        uint64_t total{};
        for (const auto& mem : memory)
        {
            total += mem.second;
        }

        std::cout << total << std::endl;
    }

    uint64_t ParseSingleMask(const std::string& input, char letter)
    {
        auto maskStr = input.substr(7);

        uint64_t mask{};
        for (auto i = input.size(); i > 0; --i)
        {
            char bit = input[i - 1];
            if (bit == letter)
            {
                mask |= (1ULL << (input.size() - i));
            }
        }
        return mask;
    }

    std::vector<uint64_t> ParseXMask(const std::string& input)
    {
        std::vector<uint64_t> result{};
        auto maskStr = input.substr(7);
        for (auto i = input.size(); i > 0; --i)
        {
            char bit = input[i - 1];
            if (bit == 'X')
            {
                result.emplace_back(input.size() - i);
            }
        }
        return result;
    }

    void Part2()
    {
        std::vector<string> commands{};
        std::string input;
        while (std::getline(std::cin, input))
        {
            commands.emplace_back(move(input));
        }

        // address, value
        map<uint64_t, uint64_t> memory{};

        uint64_t zeroMask{};
        uint64_t oneMask{};
        vector<uint64_t> xMask{};
        for (const auto& command : commands)
        {
            if (command[1] == 'a')
            {
                // ZeroMask == unchanged so don't bother parsing it.
                oneMask = ParseSingleMask(command, '1');
                xMask = ParseXMask(command);
                continue;
            }

            const auto openingBracket = command.find('[');
            const auto closingBracket = command.find(']');
            uint64_t originalAddress = std::atoll(command.substr(openingBracket + 1, closingBracket - openingBracket - 1).c_str());
            uint64_t value = std::atoi(command.substr(command.find('=') + 2).c_str());

            uint64_t address{originalAddress};
            address = address | oneMask;

            // Use a counter as a bitfield for which of the floating bits are active.
            const auto combinations = 1ULL << xMask.size();
            for (auto i = 0ULL; i < combinations; ++i)
            {
                uint64_t addressCopy{address};
                for (auto j = 0UL; j < xMask.size(); ++j)
                {
                    // If the counter has the j'th bit set then this floating bit is set.
                    if (i & (1ULL << j))
                    {
                        addressCopy = addressCopy | (1ULL << xMask[j]);
                    }
                    else
                    {
                        addressCopy = addressCopy & (~(1ULL << xMask[j]));
                    }
                }

                memory[addressCopy] = value;
            }
        }

        uint64_t total{};
        for (const auto& mem : memory)
        {
            total += mem.second;
        }

        std::cout << total << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
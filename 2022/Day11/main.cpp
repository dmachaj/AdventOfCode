#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
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
    struct Monkey
    {
        std::vector<uint64_t> items;
        bool isAdd{false};
        bool operandIsSelf{false};
        uint64_t operand{};
        uint64_t divisor{};
        uint64_t trueTarget{};
        uint64_t falseTarget{};
        uint64_t itemsTouched{};
    };

    std::vector<Monkey> ParseInput()
    {
        std::vector<Monkey> monkeys;

        std::string input;
        while (std::getline(std::cin, input))
        {
            // first line doesn't matter.  Index is the name.
            std::getline(std::cin, input);

            Monkey current;
            auto itemsRaw = input.substr(input.find(':') + 2);
            std::istringstream itemsStream(itemsRaw);
            std::string itemRaw;
            while (std::getline(itemsStream, itemRaw, ','))
            {
                current.items.emplace_back(std::atoi(itemRaw.c_str()));
            }

            std::getline(std::cin, input);
            current.isAdd = (input.find('+') != std::string::npos);
            auto operandStr = current.isAdd ? input.substr(input.find('+') + 1) : input.substr(input.find('*') + 1);
            if (operandStr == " old")
            {
                current.operandIsSelf = true;
            }
            else
            {
                current.operand = std::atoi(operandStr.c_str());
            }

            std::getline(std::cin, input);
            current.divisor = std::atoi(input.substr(input.find("by ") + 3).c_str());

            std::getline(std::cin, input);
            current.trueTarget = std::atoi(input.substr(input.find("monkey ") + 7).c_str());

            std::getline(std::cin, input);
            current.falseTarget = std::atoi(input.substr(input.find("monkey ") + 7).c_str());

            monkeys.emplace_back(current);

            std::cin.get(); // eat newline
        }

        return monkeys;
    }

    void RunSimulation(std::vector<Monkey>& monkeys, uint64_t roundCount, uint64_t worryDivisor)
    {
        for (auto round = 0UL; round < roundCount; ++round)
        {
            for (auto i = 0UL; i < monkeys.size(); ++i)
            {
                auto& monkey = monkeys[i];
                monkey.itemsTouched += monkey.items.size();

                for (uint64_t item: monkey.items)
                {
                    uint64_t itemAfter{item};
                    if (monkey.isAdd)
                    {
                        itemAfter += monkey.operandIsSelf ? item : monkey.operand;
                    }
                    else
                    {
                        itemAfter *= monkey.operandIsSelf ? item : monkey.operand;
                    }

                    // This is kindof a stupid hack.  For part 1 we always divide by 3.
                    // For part 2 we use the least common multiple of the various divisors
                    // to modulo the worry and keep it from overflowing a uint64_t(!!).
                    if (worryDivisor > 3)
                    {
                        if (itemAfter > worryDivisor)
                        {
                            itemAfter %= worryDivisor;
                        }
                    }
                    else
                    {
                        itemAfter /= worryDivisor;
                    }

                    uint64_t targetIndex{};
                    if (itemAfter % monkey.divisor == 0)
                    {
                        targetIndex = monkey.trueTarget;
                    }
                    else
                    {
                        targetIndex = monkey.falseTarget;
                    }

                    monkeys[targetIndex].items.emplace_back(itemAfter);
                }
                monkey.items.clear();
            }
        }

        std::sort(monkeys.begin(), monkeys.end(), [](auto& left, auto& right) { return left.itemsTouched > right.itemsTouched; });

        std::cout << (monkeys[0].itemsTouched * monkeys[1].itemsTouched) << std::endl;
    }

    void Part1()
    {
        auto monkeys = ParseInput();
        RunSimulation(monkeys, 20, 3);
    }

    uint64_t LowestCommonDenominator(uint64_t first, uint64_t second)
    {
        while (true)
        {
            if (first == 0) { return second; }
            second %= first;
            if (second == 0) { return first; }
            first %= second;
        }
    }

    uint64_t LeastCommonMultiple(uint64_t first, uint64_t second)
    {
        return (first * second) / LowestCommonDenominator(first, second);
    }

    void Part2()
    {
        auto monkeys = ParseInput();

        uint64_t lowestCommon{1};
        for (const auto& monkey : monkeys)
        {
            lowestCommon = LeastCommonMultiple(lowestCommon, monkey.divisor);
        }

        RunSimulation(monkeys, 10000UL, lowestCommon);
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
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
    struct Rule
    {
        enum class Type
        {
            Literal,
            Recursive
        };

        Rule() = default;

        Rule(uint32_t id, Type type, std::vector<uint32_t> leftRules, std::vector<uint32_t> rightRules, char value):
            id(id), type(type), leftRules(leftRules), rightRules(rightRules), value(value)
        {}

        Rule(const std::string& input)
        {
            id = (uint32_t)std::atoi(input.substr(0, input.find(":")).c_str());

            if (input.find("\"") != string::npos)
            {
                type = Type::Literal;
                value = input[input.find("\"") + 1];
                return;
            }

            bool left{true};
            type = Type::Recursive;
            std::istringstream stream(input.substr(input.find(":") + 2).c_str());
            std::string temp;
            while (std::getline(stream, temp, ' '))
            {
                if (temp[0] == '|')
                {
                    left = false;
                }
                else if (left)
                {
                    leftRules.emplace_back((uint32_t)std::atoi(temp.c_str()));
                }
                else
                {
                    rightRules.emplace_back((uint32_t)std::atoi(temp.c_str()));
                }
            }
        }

        uint32_t id{};
        Type type{};
        std::vector<uint32_t> leftRules; // if recursive
        std::vector<uint32_t> rightRules; // if recursive
        char value{}; // if literal
    };

    std::set<uint32_t> ConsumeLettersRecursive(
        const std::string& currentLine, const std::unordered_map<uint32_t, Rule>& allRules, 
        const Rule& currentRule, uint32_t currentOffset)
    {
        // Ran past the end of the buffer
        if (currentOffset >= currentLine.size())
        {
            return {};
        }

        // Literals nibble exactly their character, or return an empty set
        if (currentRule.type == Rule::Type::Literal)
        {
            if (currentLine[currentOffset] == currentRule.value)
            {
                return { currentOffset + 1 };
            }
            else
            {
                return {};
            }
        }

        std::set<uint32_t> leftResults{currentOffset};
        for (auto rule : currentRule.leftRules)
        {
            auto resultsCopy = leftResults;
            leftResults.clear();

            auto nextRule = (allRules.find(rule))->second;
            for (auto offset : resultsCopy)
            {
                const auto options = ConsumeLettersRecursive(currentLine, allRules, nextRule, offset);
                leftResults.insert(options.begin(), options.end());
            }

            if (leftResults.empty())
            {
                break; // no point matching when we've already failed out
            }
        }

        std::set<uint32_t> rightResults;
        if (!currentRule.rightRules.empty())
        {
            rightResults.emplace(currentOffset);
            for (auto rule : currentRule.rightRules)
            {
                auto resultsCopy = rightResults;
                rightResults.clear();

                auto nextRule = (allRules.find(rule))->second;
                for (auto offset : resultsCopy)
                {
                    const auto options = ConsumeLettersRecursive(currentLine, allRules, nextRule, offset);
                    rightResults.insert(options.begin(), options.end());
                }

                if (rightResults.empty())
                {
                    break; // no point matching when we've already failed out
                }
            }
        }

        leftResults.insert(rightResults.begin(), rightResults.end());
        return leftResults;
    }

    void Part1()
    {
        std::string input;
        std::vector<Rule> rules;
        while (std::getline(std::cin, input))
        {
            if (input.empty()) break;
            rules.emplace_back(Rule(input));
        }

        std::unordered_map<uint32_t, Rule> allRules;
        for (const auto& rule : rules)
        {
            allRules.emplace(rule.id, rule);
        }

        std::vector<std::string> inputs;
        while (std::getline(std::cin, input))
        {
            inputs.emplace_back(input);
        }

        const auto rule0 = allRules[0];

        uint32_t result = std::accumulate(inputs.begin(), inputs.end(), 0UL, [allRules, rule0](uint32_t sum, const std::string& input)
        {
            const auto matchingOffsets = ConsumeLettersRecursive(input, allRules, rule0, 0UL);
            if (matchingOffsets.find(input.size()) != matchingOffsets.end())
            {
                return sum + 1;
            }
            else
            {
                return sum;
            }
            
        });

        std::cout << result << std::endl;
    }

    void Part2()
    {
        std::string input;
        std::vector<Rule> rules;
        while (std::getline(std::cin, input))
        {
            if (input.empty()) break;
            rules.emplace_back(Rule(input));
        }

        std::unordered_map<uint32_t, Rule> allRules;
        for (const auto& rule : rules)
        {
            allRules.emplace(rule.id, rule);
        }
        allRules[8] = Rule(8, Rule::Type::Recursive, {42}, {42, 8}, 0);
        allRules[11] = Rule(11, Rule::Type::Recursive, {42, 31}, {42, 11, 31}, 0);

        std::vector<std::string> inputs;
        while (std::getline(std::cin, input))
        {
            inputs.emplace_back(input);
        }

        const auto rule0 = allRules[0];

        uint32_t result = std::accumulate(inputs.begin(), inputs.end(), 0UL, [allRules, rule0](uint32_t sum, const std::string& input)
        {
            const auto matchingOffsets = ConsumeLettersRecursive(input, allRules, rule0, 0UL);
            if (matchingOffsets.find(input.size()) != matchingOffsets.end())
            {
                return sum + 1;
            }
            else
            {
                return sum;
            }
            
        });

        std::cout << result << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
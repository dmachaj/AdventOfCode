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

        Rule(uint32_t id, Type type, std::vector<std::vector<uint32_t>> ruleOptions, char value):
            id(id), type(type), ruleOptions(ruleOptions), value(value)
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

            type = Type::Recursive;
            std::istringstream stream(input.substr(input.find(":") + 2).c_str());
            std::string temp;
            std::vector<uint32_t> rulesTemp;
            while (std::getline(stream, temp, ' '))
            {
                if (temp[0] == '|')
                {
                    ruleOptions.emplace_back(std::move(rulesTemp));
                    rulesTemp = {};
                }
                else
                {
                    rulesTemp.emplace_back((uint32_t)std::atoi(temp.c_str()));
                }
            }

            if (!rulesTemp.empty())
            {
                ruleOptions.emplace_back(std::move(rulesTemp));
            }
        }

        uint32_t id{};
        Type type{};
        std::vector<std::vector<uint32_t>> ruleOptions; // if recursive
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

        std::set<uint32_t> results{};
        for (auto ruleset : currentRule.ruleOptions)
        {
            std::set<uint32_t> currentRuleResults{currentOffset};
            for (auto rule : ruleset)
            {
                auto resultsCopy = currentRuleResults;
                currentRuleResults.clear();

                auto nextRule = (allRules.find(rule))->second;
                for (auto offset : resultsCopy)
                {
                    const auto options = ConsumeLettersRecursive(currentLine, allRules, nextRule, offset);
                    currentRuleResults.insert(options.begin(), options.end());
                }

                if (currentRuleResults.empty())
                {
                    break; // no point matching when we've already failed out
                }
            }

            results.insert(currentRuleResults.begin(), currentRuleResults.end());
        }
        return results;
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
        allRules[8] = Rule(8, Rule::Type::Recursive, {{42}, {42, 8}}, 0);
        allRules[11] = Rule(11, Rule::Type::Recursive, {{42, 31}, {42, 11, 31}}, 0);

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
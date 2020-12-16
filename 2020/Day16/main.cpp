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

namespace
{
    pair<uint32_t, uint32_t> ParseConstraint(const std::string& input)
    {
        const auto first = std::atoi(input.substr(0, input.find("-")).c_str());
        const auto second = std::atoi(input.substr(input.find("-") + 1).c_str());
        return make_pair(first, second);
    }

    vector<uint32_t> ParseTicket(const std::string& input)
    {
        vector<uint32_t> result;
        string num;
        std::istringstream inputStream(input);
        while (std::getline(inputStream, num, ','))
        {
            result.emplace_back(std::atoi(num.c_str()));
        }
        return result;
    }

    void Part1()
    {
        std::vector<std::string> constraints;
        std::string input;
        while (std::getline(std::cin, input))
        {
            if (input.empty()) break;
            constraints.emplace_back(move(input));
        }

        std::getline(std::cin, input);
        if (input != "your ticket:") throw 0;
        std::string myTicket;
        std::getline(std::cin, myTicket);

        std::getline(std::cin, input);
        if (!input.empty()) throw 0;

        std::getline(std::cin, input);
        if (input != "nearby tickets:") throw 0;
        std::vector<std::string> otherTickets;
        while (std::getline(std::cin, input))
        {
            if (input.empty()) break;
            otherTickets.emplace_back(move(input));
        }

        vector<pair<uint32_t, uint32_t>> parsedConstraints;
        for (const auto& constraint : constraints)
        {
            const auto firstNum = constraint.find(":") + 2;
            const auto or = constraint.find(" or ");
            const auto first = constraint.substr(firstNum, or - firstNum);
            const auto second = constraint.substr(or + 4);
            parsedConstraints.emplace_back(ParseConstraint(first));
            parsedConstraints.emplace_back(ParseConstraint(second));
        }

        uint32_t runningTotal{};
        for (auto&& ticket : otherTickets)
        {
            const auto values = ParseTicket(ticket);
            for (auto&& value : values)
            {
                bool valid{false};
                for (auto&& constraint: parsedConstraints)
                {
                    if ((value >= constraint.first) && (value <= constraint.second))
                    {
                        valid = true;
                        break;
                    }
                }

                if (!valid)
                {
                    runningTotal += value;
                }
            }
        }

        std::cout << runningTotal << std::endl;
    }

    // Avert ye eyes, all who go here.  This is an UGLY one.
    void Part2()
    {
        std::vector<std::string> constraints;
        std::string input;
        while (std::getline(std::cin, input))
        {
            if (input.empty()) break;
            constraints.emplace_back(move(input));
        }

        std::getline(std::cin, input);
        if (input != "your ticket:") throw 0;
        std::string myTicket;
        std::getline(std::cin, myTicket);

        std::getline(std::cin, input);
        if (!input.empty()) throw 0;

        std::getline(std::cin, input);
        if (input != "nearby tickets:") throw 0;
        std::vector<std::string> otherTickets;
        while (std::getline(std::cin, input))
        {
            if (input.empty()) break;
            otherTickets.emplace_back(move(input));
        }

        struct TicketConstraint
        {
            std::string name;
            pair<uint32_t, uint32_t> valid1;
            pair<uint32_t, uint32_t> valid2;
            bool operator==(const TicketConstraint& other)
            {
                return (name == other.name) && (valid1 == other.valid1) && (valid2 == other.valid2);
            }
        };

        vector<TicketConstraint> parsedConstraints;
        for (const auto& constraint : constraints)
        {
            const auto name = constraint.substr(0, constraint.find(":"));
            const auto firstNum = constraint.find(":") + 2;
            const auto or = constraint.find(" or ");
            const auto first = constraint.substr(firstNum, or - firstNum);
            const auto second = constraint.substr(or + 4);
            const auto firstValid = ParseConstraint(first);
            const auto secondValid = ParseConstraint(second);

            TicketConstraint value =
            {
                name,
                firstValid,
                secondValid
            };
            parsedConstraints.emplace_back(move(value));
        }
        const auto columnCount = parsedConstraints.size();

        vector<vector<uint32_t>> validTickets;
        for (auto&& ticket : otherTickets)
        {
            bool allValid{true};
            const auto values = ParseTicket(ticket);
            for (auto&& value : values)
            {
                bool valid{false};
                for (auto&& constraint: parsedConstraints)
                {
                    if ((value >= constraint.valid1.first) && (value <= constraint.valid1.second))
                    {
                        valid = true;
                        break;
                    }
                    else if ((value >= constraint.valid2.first) && (value <= constraint.valid2.second))
                    {
                        valid = true;
                        break;
                    }
                }

                if (!valid)
                {
                    allValid = false;
                    break;
                }
            }

            if (allValid)
            {
                validTickets.emplace_back(values);
            }
        }

        const auto CanMatch = [](const TicketConstraint& constraint, const vector<vector<uint32_t>>& validTickets, uint32_t column) -> bool
        {
            for (auto&& values : validTickets)
            {
                const auto currentValue = values[column];
                if (((currentValue >= constraint.valid1.first) && (currentValue <= constraint.valid1.second) )||
                    ((currentValue >= constraint.valid2.first) && (currentValue <= constraint.valid2.second)))
                {
                    // valid
                }
                else
                {
                    return false;
                }
            }
            return true;
        };

        map<uint32_t, vector<TicketConstraint>> constraintPossibilities;
        for (auto i = 0UL; i < columnCount; ++i)
        {
            for (auto&& constraint : parsedConstraints)
            {
                if (CanMatch(constraint, validTickets, i))
                {
                    constraintPossibilities[i].push_back(constraint);
                }
            }
        }

        const auto Solved = [](const map<uint32_t, vector<TicketConstraint>>& possibilities) -> bool
        {
            for (auto&& entry : possibilities)
            {
                if (!entry.second.empty())
                {
                    return false;
                }
            }
            return true;
        };

        vector<TicketConstraint> orderedConstraints(columnCount);
        while (!Solved(constraintPossibilities))
        {
            for (auto& possibilities : constraintPossibilities)
            {
                if (possibilities.second.size() == 1)
                {
                    orderedConstraints[possibilities.first] = possibilities.second[0];
                    possibilities.second.clear();

                    for (auto& otherPossibilities : constraintPossibilities)
                    {
                        const auto iter = std::find(otherPossibilities.second.begin(), otherPossibilities.second.end(), orderedConstraints[possibilities.first]);
                        if (iter != otherPossibilities.second.end())
                        {
                            otherPossibilities.second.erase(iter);
                        }
                    }
                }
            }
        }

        vector<uint32_t> results;
        const auto myTicketParsed = ParseTicket(myTicket);
        for (auto i = 0UL; i < orderedConstraints.size(); ++i)
        {
            if (orderedConstraints[i].name.find("departure") != string::npos)
            {
                results.emplace_back(myTicketParsed[i]);
            }
        }

        if (results.size() == 0) // sample input
        {
            std::cout << 0 << std::endl;
            return;
        }

        uint64_t result = results[0];
        for (auto i = 1UL; i < results.size(); ++i)
        {
            result *= results[i];
        }

        std::cout << result << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
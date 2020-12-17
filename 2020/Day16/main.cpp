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
    struct TicketConstraint
    {
        TicketConstraint() = default;
        TicketConstraint(const std::string& constraint)
        {
            name = constraint.substr(0, constraint.find(":"));

            const auto firstNum = constraint.find(":") + 2;
            const auto or = constraint.find(" or ");

            const auto first = constraint.substr(firstNum, or - firstNum);
            const auto second = constraint.substr(or + 4);

            firstValid = ParseConstraint(first);
            secondValid = ParseConstraint(second);
        }

        bool operator==(const TicketConstraint& other)
        {
            return (name == other.name) && (firstValid == other.firstValid) && (secondValid == other.secondValid);
        }

        bool IsValid(uint32_t value) const
        {
            return (((value >= firstValid.first) && (value <= firstValid.second) )||
                    ((value >= secondValid.first) && (value <= secondValid.second)));
        }

        std::string name;
        pair<uint32_t, uint32_t> firstValid;
        pair<uint32_t, uint32_t> secondValid;

    private:
        pair<uint32_t, uint32_t> ParseConstraint(const std::string& input)
        {
            const auto first = std::atoi(input.substr(0, input.find("-")).c_str());
            const auto second = std::atoi(input.substr(input.find("-") + 1).c_str());
            return make_pair(first, second);
        }
    };

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

        std::vector<TicketConstraint> parsedConstraints;
        for (const auto& constraint : constraints)
        {
            parsedConstraints.emplace_back(TicketConstraint(constraint));
        }

        uint32_t runningTotal{};
        for (auto&& ticket : otherTickets)
        {
            const auto values = ParseTicket(ticket);
            for (auto&& value : values)
            {
                bool valid = std::any_of(parsedConstraints.begin(), parsedConstraints.end(), [value](const TicketConstraint& constraint)
                {
                    return constraint.IsValid(value);
                });

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
        std::string input;

        std::vector<std::string> constraints;
        while (std::getline(std::cin, input))
        {
            if (input.empty()) break;
            constraints.emplace_back(move(input));
        }

        std::string myTicket;
        std::vector<std::string> otherTickets;
        {
            std::getline(std::cin, input);
            if (input != "your ticket:") throw 0;
            std::getline(std::cin, myTicket);

            std::getline(std::cin, input);
            if (!input.empty()) throw 0;

            std::getline(std::cin, input);
            if (input != "nearby tickets:") throw 0;
            while (std::getline(std::cin, input))
            {
                if (input.empty()) break;
                otherTickets.emplace_back(move(input));
            }
        }

        vector<TicketConstraint> parsedConstraints;
        for (const auto& constraint : constraints)
        {
            parsedConstraints.emplace_back(TicketConstraint(constraint));
        }
        const auto columnCount = parsedConstraints.size();

        vector<vector<uint32_t>> validTickets;
        for (auto&& ticket : otherTickets)
        {
            const auto values = ParseTicket(ticket);
            const bool allValid = std::all_of(values.begin(), values.end(), [parsedConstraints](uint32_t value)
            {
                bool valid = std::any_of(parsedConstraints.begin(), parsedConstraints.end(), [value](const TicketConstraint& constraint)
                {
                    return constraint.IsValid(value);
                });
                return valid;
            });

            if (allValid)
            {
                validTickets.emplace_back(values);
            }
        }

        map<uint32_t, vector<TicketConstraint>> constraintPossibilities;
        for (auto i = 0UL; i < columnCount; ++i)
        {
            for (auto&& constraint : parsedConstraints)
            {
                const bool allMatches = std::all_of(validTickets.begin(), validTickets.end(), [i, constraint](const auto& ticket)
                {
                    return constraint.IsValid(ticket[i]);
                });

                if (allMatches)
                {
                    constraintPossibilities[i].push_back(constraint);
                }
            }
        }

        const auto Solved = [](const map<uint32_t, vector<TicketConstraint>>& possibilities) -> bool
        {
            return std::all_of(possibilities.begin(), possibilities.end(), [](const auto& entry)
            {
                return entry.second.empty();
            });
        };

        vector<TicketConstraint> orderedConstraints(columnCount);
        while (!Solved(constraintPossibilities))
        {
            for (auto& possibilities : constraintPossibilities)
            {
                if (possibilities.second.size() == 1)
                {
                    orderedConstraints[possibilities.first] = possibilities.second[0];
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

        uint64_t result{1};
        for (auto&& value : results)
        {
            result *= value;
        }

        std::cout << result << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
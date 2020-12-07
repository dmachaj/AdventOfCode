#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include "HelpfulInfrastructure.h"

using namespace std::literals;

#pragma warning (disable: 4267) // size_t <-> uint

namespace
{
    void Part1()
    {
        struct Option
        {
            std::string outer;
            std::string inner;
        };
        std::vector<Option> options{};

        std::string input;
        while (std::getline(std::cin, input))
        {
            if (input.find("no other bags") != std::string::npos)
            {
                // this is useless
                continue;
            }

            std::string outer = input.substr(0, input.find("bags") - 1);
            std::string inner = input.substr(input.find("contain") + 7);
            // std::cerr << outer << std::endl << "  " << inner << std::endl;

            if (outer.find("shiny gold") != std::string::npos)
            {
                continue;
            }

            Option current{};
            current.outer = std::move(outer);
            current.inner = std::move(inner);
            options.emplace_back(std::move(current));
        }

        std::set<std::string> colors;
        for (const auto& option : options)
        {
            colors.emplace(option.outer);
        }

        std::set<std::string> colorsToGold{"shiny gold"};
        while (!colors.empty())
        {
            const auto before { colorsToGold.size() };
            for (const auto color : colors)
            {
                const auto iter = std::find_if(options.begin(), options.end(), [color](const auto& option) { return option.outer == color; });
                for (const auto i : colorsToGold)
                {
                    if (iter->inner.find(i) != std::string::npos)
                    {
                        colorsToGold.emplace(iter->outer);
                        break;
                    }
                }
            }

            if (colorsToGold.size() == before)
            {
                break; // no more progress to be made.
            }

            for (const auto i : colorsToGold)
            {
                colors.erase(i);
            }
        }

        std::cout << colorsToGold.size() - 1 << std::endl;
    }

    uint32_t BagsNeededRecursive(const std::vector<std::string>& allLines, std::string color, int indent = 0)
    {
        const auto current = std::find_if(allLines.begin(), allLines.end(), [color](const auto& current) { return current.find(color + " bags contain") != std::string::npos; });

        if (current->find("no other bags") != std::string::npos)
        {
            return 1; // end recursion
        }

        // std::cerr << std::string(4 * indent, ' ') << color << std::endl;

        uint32_t total{1}; // count the bag itself on top of all that it contains
        auto inner = current->substr(current->find("contain") + 8);
        while (inner.find("bag") != std::string::npos)
        {
            int quantity = std::atoi(&inner[0]);
            std::string innerColor = inner.substr(2, inner.find("bag") - 3);
            const auto recurse = BagsNeededRecursive(allLines, innerColor, indent + 1);
            // std::cerr << std::string(4 * (indent + 1), ' ') << quantity << " * " << innerColor << "(" << recurse << ")" << std::endl;
            total += quantity * recurse;

            if (inner.find(",") == std::string::npos) { break; }
            else
            {
                inner = inner.substr(inner.find(",") + 2);
            }
        }

        return total;
    }

    void Part2()
    {
        std::vector<std::string> allLines{};
        std::string input;
        while (std::getline(std::cin, input))
        {
            allLines.emplace_back(std::move(input));
        }

        const auto answer = BagsNeededRecursive(allLines, "shiny gold") -1; // subtract the gold bag itself
        std::cout << answer << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include "HelpfulInfrastructure.h"

using namespace std::literals;

#pragma warning (disable: 4267) // size_t <-> uint

namespace
{
    uint32_t ProcessEntry(std::string input)
    {
        // std::cerr << input << std::endl;
        std::set<char> theSet{};
        for (const auto letter : input)
        {
            theSet.emplace(letter);
        }
        return theSet.size();
    }

    void Part1()
    {
        uint32_t runningTotal{};

        std::vector<std::string> allLines{};
        {
            std::string input;
            while (std::getline(std::cin, input))
            {
                allLines.emplace_back(std::move(input));
            }
            allLines.emplace_back(std::string()); // add a blank line to the end to ease the next loop
        }

        std::string current;
        for(auto i = 0; i < allLines.size(); ++i)
        {
            if (allLines[i].empty())
            {
                runningTotal += ProcessEntry(current);
                current.clear();
                continue;
            }
            current += allLines[i];
        }

        std::cout << runningTotal << std::endl;
    }

    uint32_t ProcessEntries(std::vector<std::string> input)
    {
        uint32_t result{};
        for (char letter = 'a'; letter <= 'z'; ++letter)
        {
            bool found = true;
            for (const auto str : input)
            {
                if (str.find(letter) == std::string::npos)
                {
                    found = false;
                    continue;
                }
            }

            if (found) { ++result; }
        }
        return result;
    }

    void Part2()
    {
        uint32_t runningTotal{};

        std::vector<std::string> allLines{};
        {
            std::string input;
            while (std::getline(std::cin, input))
            {
                allLines.emplace_back(std::move(input));
            }
            allLines.emplace_back(std::string()); // add a blank line to the end to ease the next loop
        }

        std::vector<std::string> current;
        for(auto i = 0; i < allLines.size(); ++i)
        {
            if (allLines[i].empty())
            {
                runningTotal += ProcessEntries(current);
                current.clear();
                continue;
            }
            current.emplace_back(allLines[i]);
        }

        std::cout << runningTotal << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
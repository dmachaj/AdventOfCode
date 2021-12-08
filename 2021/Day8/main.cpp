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
    uint32_t CountSpecificDigits(const std::string& input)
    {
        std::string first, second, third, fourth;
        std::istringstream instream(input);
        instream >> first >> second >> third >> fourth;

        uint32_t result{};
        if (first.length() == 7 || first.length() == 2 || first.length() == 3 || first.length() == 4)
        {
            ++result;
        }

        if (second.length() == 7 || second.length() == 2 || second.length() == 3 || second.length() == 4)
        {
            ++result;
        }

        if (third.length() == 7 || third.length() == 2 || third.length() == 3 || third.length() == 4)
        {
            ++result;
        }

        if (fourth.length() == 7 || fourth.length() == 2 || fourth.length() == 3 || fourth.length() == 4)
        {
            ++result;
        }

        return result;
    }

    void Part1()
    {
        std::string input;
        uint32_t result{};
        while (std::getline(std::cin, input))
        {
            const auto output = input.substr(input.find('|') + 1);
            result += CountSpecificDigits(output);
        }
        std::cout << result << std::endl;
    }

    uint32_t SolveNumber(
        const std::string& input, 
        const std::vector<std::string>& samples,
        const std::string& one,
        const std::string& seven,
        const std::string& four,
        const std::string& eight)
    {
        // Easy answers return quick
        if (input.length() == one.length()) return 1;
        else if (input.length() == seven.length()) return 7;
        else if (input.length() == four.length()) return 4;
        else if (input.length() == eight.length()) return 8;
        
        // Take advantage of the lengths limiting the possibilities and the fact that all of the unique numbers always
        // appear in the sample set.  That means we can cross-reference between 3 possibilities and the known numbers
        // to tell them apart.  Sneaky shortcut for the win.
        if (input.length() == 5) // 2 or 3 or 5
        {
            if (std::all_of(seven.begin(), seven.end(), [input](char c) { return input.find(c) != std::string::npos; }))
            {
                return 3;
            }

            const auto fourOverlap = std::accumulate(four.begin(), four.end(), 0UL, [input](auto sum, char c)
            {
                return (input.find(c) != std::string::npos) ? sum + 1 : sum;
            });
            if (fourOverlap == 2)
            {
                return 2;
            }
            else if (fourOverlap == 3)
            {
                return 5;
            }
        }
        else if (input.length() == 6) // 0 or 6 or 9
        {
            if (std::all_of(four.begin(), four.end(), [input](char c) { return input.find(c) != std::string::npos; }))
            {
                return 9;
            }
            else if (std::all_of(seven.begin(), seven.end(), [input](char c) { return input.find(c) != std::string::npos; }))
            {
                return 0;
            }
            return 6;
        }
        else
        {
            throw std::exception();
        }
        return 0;
    }

    uint32_t SolveLine(const std::string& input)
    {
        const auto samplesStr = input.substr(0, input.find('|'));
        const auto outputStr = input.substr(input.find('|') + 1);

        std::vector<std::string> samples;
        {
            std::istringstream instream(samplesStr);
            std::string temp;
            while (std::getline(instream, temp, ' '))
            {
                samples.emplace_back(std::move(temp));
            }
        }

        auto one = std::find_if(samples.begin(), samples.end(), [](const auto& sample) { return sample.length() == 2; });
        auto seven = std::find_if(samples.begin(), samples.end(), [](const auto& sample) { return sample.length() == 3; });
        auto four = std::find_if(samples.begin(), samples.end(), [](const auto& sample) { return sample.length() == 4; });
        auto eight = std::find_if(samples.begin(), samples.end(), [](const auto& sample) { return sample.length() == 7; });

        // Every test case has at least a one a seven a four and an eight with none of them missing.
        if (one == samples.end() || seven == samples.end() || four == samples.end() || eight == samples.end()) throw std::exception();

        std::string firstOutput, secondOutput, thirdOutput, fourthOutput;
        {
            std::istringstream instream(outputStr);
            instream >> firstOutput >> secondOutput >> thirdOutput >> fourthOutput;
        }

        uint32_t result { (SolveNumber(firstOutput, samples, *one, *seven, *four, *eight) * 1000) + 
                          (SolveNumber(secondOutput, samples, *one, *seven, *four, *eight) * 100) +
                          (SolveNumber(thirdOutput, samples, *one, *seven, *four, *eight) * 10) +
                           SolveNumber(fourthOutput, samples, *one, *seven, *four, *eight) };
        // std::cerr << result << std::endl;

        return result;
    }

    void Part2()
    {
        std::string input;
        uint32_t result{};
        while (std::getline(std::cin, input))
        {
            result += SolveLine(input);
        }
        std::cout << result << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
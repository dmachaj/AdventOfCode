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
    bool CanReact(char left, char right)
    {
        return (std::isupper(left) && std::islower(right) && (std::toupper(right) == left)) ||
               (std::islower(left) && std::isupper(right) && (std::toupper(left) == right));
    }

    std::string FullyReactPolymer(std::string input)
    {
        while (true)
        {
            const auto beginLength = input.length();

            for (auto iter = input.begin(); iter != input.end(); ++iter)
            {
                if (*iter == '\0') break;

                if ((iter + 1) != input.end())
                {
                    if (CanReact(*iter, *(iter + 1)))
                    {
                        iter = input.erase(iter);
                        iter = input.erase(iter);
                    }
                }
            }

            const auto endLength = input.length();

            if (beginLength == endLength)
            {
                break;
            }
        }

        return input;
    }

    void Part1()
    {
        std::string input;
        std::getline(std::cin, input);

        const auto result = FullyReactPolymer(input);

        std::cout << result.length() << std::endl;
    }

    void Part2()
    {
        std::string input;
        std::getline(std::cin, input);

        std::unordered_map<char, uint32_t> results;
        for (auto c = 'a'; c <= 'z'; ++c)
        {
            if (input.find(c) == std::string::npos) continue;

            auto inputCopy = input;

            auto endIter = std::remove(inputCopy.begin(), inputCopy.end(), c);
            inputCopy.erase(endIter, inputCopy.end());

            endIter = std::remove(inputCopy.begin(), inputCopy.end(), std::toupper(c));
            inputCopy.erase(endIter, inputCopy.end());

            const auto result = FullyReactPolymer(inputCopy);
            results[c] = result.length();
        }

        const auto bestReaction = *std::min_element(results.begin(), results.end(), [](const auto& left, const auto& right) { return left.second < right.second; });
        std::cout << bestReaction.second << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <iterator>
#include <map>
#include <unordered_map>
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
    void Part1()
    {
        std::vector<uint32_t> inputNums{};
        std::string input;
        while (std::getline(std::cin, input, ','))
        {
            inputNums.emplace_back(std::atoi(input.c_str()));
        }

        std::vector<uint32_t> allMoves(inputNums);
        for (auto i = allMoves.size(); i < 2020; ++i)
        {
            const auto lastNum = allMoves[i - 1];
            auto iter = std::find(allMoves.rbegin() + 1, allMoves.rend(), lastNum);
            if (iter == allMoves.rend())
            {
                allMoves.emplace_back(0);
            }
            else
            {
                auto distance = (uint32_t)std::distance(allMoves.rbegin(), iter);
                allMoves.emplace_back(distance);
            }
        }

        std::cout << allMoves[allMoves.size() - 1] << std::endl;
    }

    void Part2()
    {
        std::vector<uint64_t> inputNums{};
        std::string input;
        while (std::getline(std::cin, input, ','))
        {
            inputNums.emplace_back(std::atoi(input.c_str()));
        }

        // key = number, value = last appearance
        std::unordered_map<uint64_t, uint64_t> allMoves{};
        for (auto i = 0; i < inputNums.size() - 1; ++i)
        {
            allMoves[inputNums[i]] = i;
        }

        auto lastNumber = *inputNums.rbegin();
        // We update the lastNumber for the next iteration so we need to actually run the loop one LESS time than expected.
        // Don't ask how long this took me to debug and figure out :(
        for (auto i = (uint64_t)inputNums.size() - 1; i < (30000000ULL - 1ULL); ++i)
        {
            auto iter = allMoves.find(lastNumber);
            if (iter == allMoves.end())
            {
                allMoves[lastNumber] = i;
                lastNumber = 0ULL;
            }
            else
            {
                auto distance = i - iter->second;
                iter->second = i;
                lastNumber = distance;
            }
        }

        std::cout << lastNumber << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
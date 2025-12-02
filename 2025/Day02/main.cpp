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

inline bool IsValid(uint64_t num)
{
    char str[20]{}; 
    sprintf_s(str, sizeof(str), "%lld", num);
    std::string_view view{str};

    if (view.length() % 2 != 0)
    {
        return true;
    }

    std::string_view left{str, view.length() / 2};
    std::string_view right{str + view.length() / 2};

    return !(std::equal(left.begin(), left.end(), right.begin(), right.end()));
}

uint64_t CountCases(const std::string& input)
{
    const auto dash = input.find('-');
    const auto leftStr = input.substr(0, dash);
    const uint64_t left = std::atoll(leftStr.c_str());
    const auto rightStr = input.substr(dash + 1);
    const uint64_t right = std::atoll(rightStr.c_str());

    uint64_t result{};
    for (auto i = left; i <= right; ++i)
    {
        if (!IsValid(i))
        {
            result += i;
        }
    }

    return result;
}

void Part1()
{
    std::string input;
    std::getline(std::cin, input);

    uint64_t total{};

    while (true)
    {
        const auto nextComma = input.find(',');
        if (nextComma != std::string::npos)
        {
            auto substr = input.substr(0, nextComma);
            total += CountCases(substr);
            input = input.substr(nextComma + 1);
        }
        else
        {
            total += CountCases(input);
            break;
        }
    }

    std::cout << total << std::endl;
}

inline bool IsValidPart2(uint64_t num)
{
    char str[20]{}; 
    sprintf_s(str, sizeof(str), "%lld", num);
    std::string_view view{str};

    // Try splitting into 2, 3, 4, etc. substrings to compare for equality
    for (auto i = 2; i < 20 && i <= view.length(); ++i)
    {
        if (view.length() % i == 0)
        {
            // String divides evenly into this many spans
            const auto numDigitsToCompare = view.length() / i;
            bool mismatch = false;
            for (auto substringToCompare = 1; substringToCompare < i; ++substringToCompare)
            {
                const auto substringStartIndex = numDigitsToCompare * substringToCompare;
                for (auto j = 0; j < numDigitsToCompare; ++j)
                {
                    if (view[j] != view[substringStartIndex + j])
                    {
                        mismatch = true;
                        break;
                    }
                }
            }

            if (!mismatch)
            {
                return false;
            }
        }
    }

    return true;
}

uint64_t CountCasesPart2(const std::string& input)
{
    const auto dash = input.find('-');
    const auto leftStr = input.substr(0, dash);
    const uint64_t left = std::atoll(leftStr.c_str());
    const auto rightStr = input.substr(dash + 1);
    const uint64_t right = std::atoll(rightStr.c_str());

    uint64_t result{};
    for (auto i = left; i <= right; ++i)
    {
        // std::cerr << i << " ";
        if (!IsValidPart2(i))
        {
            // std::cerr << "INVALID" << std::endl;
            result += i;
        }
        // else
        // {
        //     std::cerr << std::endl;
        // }
    }

    return result;
}

void Part2()
{
    std::string input;
    std::getline(std::cin, input);

    uint64_t total{};

    while (true)
    {
        const auto nextComma = input.find(',');
        if (nextComma != std::string::npos)
        {
            auto substr = input.substr(0, nextComma);
            total += CountCasesPart2(substr);
            input = input.substr(nextComma + 1);
        }
        else
        {
            total += CountCasesPart2(input);
            break;
        }
    }

    std::cout << total << std::endl;
}

int main()
{
    // std::cerr << "11 " << IsValidPart2(11) << std::endl;
    // std::cerr << "12 " << IsValidPart2(12) << std::endl;
    // std::cerr << "22 " << IsValidPart2(22) << std::endl;
    // std::cerr << "111 " << IsValidPart2(111) << std::endl;
    // std::cerr << "112 " << IsValidPart2(112) << std::endl;
    // std::cerr << "12341234 " << IsValidPart2(12341234) << std::endl;
    // std::cerr << "123123123 " << IsValidPart2(123123123) << std::endl;

    RunPart1() ? Part1() : Part2();
    return 0;
}
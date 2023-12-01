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

void Part1()
{
    uint32_t result{};
    std::string input;
    while (std::getline(std::cin, input))
    {
        const auto first = *std::find_if(input.begin(), input.end(), [](auto i) { return std::isdigit(i); });
        const auto last = *std::find_if(input.rbegin(), input.rend(), [](auto i) { return std::isdigit(i); });

        const auto val = (first - '0') * 10 + last - '0';
        result += val;
    }
    std::cout << result << std::endl;
}

void Part2()
{
    uint32_t result{};
    std::string input;
    while (std::getline(std::cin, input))
    {
        std::vector<int> digits;

        for (auto i = 0; i < input.size(); ++i)
        {
            if (std::isdigit(input[i]))
            {
                digits.emplace_back(input[i] - '0');
                continue;
            }

            const auto substr = input.substr(i);
            if (substr.find("one") == 0)
            {
                digits.emplace_back(1);
            }
            else if (substr.find("two") == 0)
            {
                digits.emplace_back(2);
            }
            else if (substr.find("three") == 0)
            {
                digits.emplace_back(3);
            }
            else if (substr.find("four") == 0)
            {
                digits.emplace_back(4);
            }
            else if (substr.find("five") == 0)
            {
                digits.emplace_back(5);
            }
            else if (substr.find("six") == 0)
            {
                digits.emplace_back(6);
            }
            else if (substr.find("seven") == 0)
            {
                digits.emplace_back(7);
            }
            else if (substr.find("eight") == 0)
            {
                digits.emplace_back(8);
            }
            else if (substr.find("nine") == 0)
            {
                digits.emplace_back(9);
            }
        }

        const auto first = digits[0];
        const auto last = *digits.rbegin();

        const auto val = (first * 10) + last;
        // std::cerr << val << std::endl;
        result += val;
    }
    std::cout << result << std::endl;
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
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
    void Part1()
    {
        uint32_t result{};

        std::string input;
        while (std::getline(std::cin, input))
        {
            auto firstDash = input.find('-');
            auto comma = input.find(',');
            auto secondDash = input.find('-', comma);

            auto x = std::atoi(input.substr(0, firstDash).c_str());
            auto y = std::atoi(input.substr(firstDash + 1, comma - firstDash).c_str());

            auto a = std::atoi(input.substr(comma + 1, secondDash - comma).c_str());
            auto b = std::atoi(input.substr(secondDash + 1).c_str());

            if ((x >= a) && (y <= b))
            {
                ++result;
                continue;
            }
            else if ((a >= x) && (b <= y))
            {
                ++result;
                continue;
            }
            
        }
        std::cout << result << std::endl;
    }

    void Part2()
    {
        uint32_t result{};

        std::string input;
        while (std::getline(std::cin, input))
        {
            auto firstDash = input.find('-');
            auto comma = input.find(',');
            auto secondDash = input.find('-', comma);

            auto x = std::atoi(input.substr(0, firstDash).c_str());
            auto y = std::atoi(input.substr(firstDash + 1, comma - firstDash).c_str());

            auto a = std::atoi(input.substr(comma + 1, secondDash - comma).c_str());
            auto b = std::atoi(input.substr(secondDash + 1).c_str());

            std::vector<uint32_t> first{};
            for (auto i = x; i <= y; ++i)
            {
                first.emplace_back(i);
            }

            std::vector<uint32_t> second{};
            for (auto i = a; i <= b; ++i)
            {
                second.emplace_back(i);
            }

            std::vector<uint32_t> overlap{};
            std::set_intersection(first.begin(), first.end(), second.begin(), second.end(), std::back_inserter(overlap));

            if (!overlap.empty())
            {
                ++result;
            }
        }
        std::cout << result << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
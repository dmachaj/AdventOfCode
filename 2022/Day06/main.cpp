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
        std::string input;
        while (std::getline(std::cin, input))
        {
            for (auto i = 0; i < input.size() - 4; ++i)
            {
                auto substr = input.substr(i, 4);
                set<char> uniques;
                for (auto c : substr)
                {
                    uniques.emplace(c);
                }

                if (uniques.size() == 4)
                {
                    std::cout << i + 4 << " ";
                    break;
                }
            }
        }
        std::cout << std::endl;
    }

    void Part2()
    {
        std::string input;
        while (std::getline(std::cin, input))
        {
            for (auto i = 0; i < input.size() - 14; ++i)
            {
                auto substr = input.substr(i, 14);
                set<char> uniques;
                for (auto c : substr)
                {
                    uniques.emplace(c);
                }

                if (uniques.size() == 14)
                {
                    std::cout << i + 14 << " ";
                    break;
                }
            }
        }
        std::cout << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
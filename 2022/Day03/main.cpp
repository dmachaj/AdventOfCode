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
    uint32_t GetCount(char c)
    {
        if (c > 'Z')
        {
            return c - 'a' + 1;
        }
        return c - 'A' + 27;
    }

    void Part1()
    {
        uint32_t total{};

        std::string input;
        while (std::getline(std::cin, input))
        {
            auto first = input.substr(0, input.length() / 2);
            auto second = input.substr(input.length() / 2);
            
            // More elegant solution (after the fact) using STL algorithm.
            std::sort(first.begin(), first.end());
            std::sort(second.begin(), second.end());
            std::vector<char> intersection;
            std::set_intersection(first.begin(), first.end(), second.begin(), second.end(), std::back_inserter(intersection));
            total += GetCount(intersection[0]);

            // Solution from the live coding session.
            // for (const auto c : first)
            // {
            //     if (second.find(c) != std::string::npos)
            //     {
            //         total += GetCount(c);
            //         break;
            //     }
            // }
        }
        std::cout << total << std::endl;
    }

    void Part2()
    {
        uint32_t total{};

        std::string input;
        while (std::getline(std::cin, input))
        {
            std::string input2;
            std::getline(std::cin, input2);
            std::string input3;
            std::getline(std::cin, input3);

            // More elegant solution (after the fact) using STL algorithm.
            std::sort(input.begin(), input.end());
            std::sort(input2.begin(), input2.end());
            std::sort(input3.begin(), input3.end());

            std::vector<char> intersection;
            std::set_intersection(input.begin(), input.end(), input2.begin(), input2.end(), std::back_inserter(intersection));

            std::vector<char> intersection2;
            std::set_intersection(intersection.begin(), intersection.end(), input3.begin(), input3.end(), std::back_inserter(intersection2));
            total += GetCount(intersection2[0]);

            // Solution from the live coding session.
            // bool exit{ false };
            // for (const auto c : input)
            // {
            //     if (exit) break;

            //     for (const auto c2 : input2)
            //     {
            //         if (exit) break;
            //         if (c != c2) continue;

            //         for (const auto c3 : input3)
            //         {
            //             if (c2 == c3)
            //             {
            //                 total += GetCount(c);
            //                 exit = true;
            //                 break;
            //             }
            //         }
            //     }
            // }
        }
        std::cout << total << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
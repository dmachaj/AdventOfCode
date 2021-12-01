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
        std::vector<uint32_t> depths;
        {
            std::string input;
            while (std::getline(std::cin, input))
            {
                depths.emplace_back(std::atoi(input.c_str()));
            }
        }
        
        uint32_t result {};
        for (auto i = 0UL; i < depths.size(); ++i)
        {
            if ((i + 1) < depths.size())
            {
                if (depths[i + 1] > depths[i])
                {
                    ++result;
                }
            }
        }

        std::cout << result << std::endl;
    }

    void Part2()
    {
        std::vector<uint32_t> depths;
        {
            std::string input;
            while (std::getline(std::cin, input))
            {
                depths.emplace_back(std::atoi(input.c_str()));
            }
        }
        
        vector<int> windows;
        {
            for (auto i = 0UL; i < depths.size(); ++i)
            {
                if (((i + 1) < depths.size()) && ((i + 2) < depths.size()))
                {
                    windows.emplace_back(depths[i] + depths[i + 1] + depths[i + 2]);
                }
            }
        }

        uint32_t result {};
        for (auto i = 0UL; i < windows.size(); ++i)
        {
            if ((i + 1) < windows.size())
            {
                if (windows[i + 1] > windows[i])
                {
                    result++;
                }
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
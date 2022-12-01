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
        std::vector<uint32_t> elves;
        elves.push_back(0);

        std::string input;
        while (std::getline(std::cin, input))
        {
            if (input.empty())
            {
                elves.push_back(0);
            }
            else
            {
                elves[elves.size() - 1] += std::atoi(input.c_str());
            }
        }

        std::cout << *max_element(elves.begin(), elves.end()) << std::endl;
    }

    void Part2()
    {
        std::vector<uint32_t> elves;
        elves.push_back(0);

        std::string input;
        while (std::getline(std::cin, input))
        {
            if (input.empty())
            {
                elves.push_back(0);
            }
            else
            {
                elves[elves.size() - 1] += std::atoi(input.c_str());
            }
        }
        
        std::sort(elves.begin(), elves.end());
        std::cout << (*elves.rbegin() + *(elves.rbegin() + 1) + *(elves.rbegin() + 2)) << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
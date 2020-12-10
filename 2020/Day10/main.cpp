#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include "HelpfulInfrastructure.h"

using namespace std; // I wouldn't normally do this
using namespace std::literals;

#pragma warning (disable: 4267) // size_t <-> uint

namespace
{
    void Part1()
    {
        vector<uint32_t> chargers{};

        std::string input;
        while (std::getline(std::cin, input))
        {
            chargers.emplace_back(std::atoi(input.c_str()));
        }
        std::sort(chargers.begin(), chargers.end());

        uint32_t device = *std::max_element(chargers.begin(), chargers.end()) + 3;
        device;

        uint32_t oneDiffs{1};
        uint32_t threeDiffs{1};

        for (auto i = 0UL; i < (chargers.size() - 1); ++i)
        {
            if ((chargers[i + 1] - chargers[i]) == 1)
            {
                oneDiffs++;
            }
            else if ((chargers[i + 1] - chargers[i]) == 3)
            {
                threeDiffs++;
            }
        }

        std::cout << (oneDiffs * threeDiffs) << std::endl;
    }

    uint64_t Recurse(const vector<uint32_t>& chargers, size_t index, map<uint32_t, uint64_t>& cache)
    {
        if (index == chargers.size()) { return 1; }

        if (cache.find(index) != cache.end())
        {
            return cache[index];
        }

        uint64_t p1 = Recurse(chargers, index + 1, cache); // can always go one level deeper
        uint64_t p2{};
        if ((index + 2) < chargers.size() && (chargers[index + 2] - chargers[index] <= 3))
        {
            p2 = Recurse(chargers, index + 2, cache);
        }
        uint64_t p3{};
        if ((index + 3) < chargers.size() && (chargers[index + 3] - chargers[index] <= 3))
        {
            p3 = Recurse(chargers, index + 3, cache);
        }
        cache[index] = p1 + p2 + p3;
        return p1 + p2 + p3;
    }

    void Part2()
    {
        vector<uint32_t> chargers{};
        chargers.push_back(0);

        std::string input;
        while (std::getline(std::cin, input))
        {
            chargers.emplace_back(std::atoi(input.c_str()));
        }
        std::sort(chargers.begin(), chargers.end());

        uint32_t device = *std::max_element(chargers.begin(), chargers.end()) + 3;
        chargers.push_back(device);

        map<uint32_t, uint64_t> cache{};
        uint64_t possibilities = Recurse(chargers, 0, cache);

        std::cout << possibilities << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
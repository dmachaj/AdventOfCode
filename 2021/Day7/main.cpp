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
    int32_t ComputeEnergy(const std::vector<int32_t>& positions, int32_t final)
    {
        int32_t result{};
        for (const auto& pos : positions)
        {
            result += std::abs(pos - final);
        }
        return result;
    }

    void Part1()
    {
        std::string input;
        std::getline(std::cin, input);
        std::istringstream instream(input);

        std::vector<int32_t> positions;
        while (std::getline(instream, input, ','))
        {
            positions.emplace_back(std::atoi(input.c_str()));
        }

        int32_t minEnergy{9999999};
        for (auto i = 0UL; i < 1150UL; ++i)
        {
            const auto energy = ComputeEnergy(positions, i);
            if (energy < minEnergy)
            {
                minEnergy = energy;
            }
        }

        std::cout << minEnergy << std::endl;
    }

    int64_t ComputeEnergyPart2(const std::vector<int64_t>& positions, int64_t final)
    {
        int64_t result{};
        for (const auto& pos : positions)
        {
            const auto distance = std::abs(pos - final);
            for (auto i = 0L; i < distance; ++i)
            {
                result += i + 1;
            }
        }
        return result;
    }

    void Part2()
    {
        std::string input;
        std::getline(std::cin, input);
        std::istringstream instream(input);

        std::vector<int64_t> positions;
        while (std::getline(instream, input, ','))
        {
            positions.emplace_back(std::atoi(input.c_str()));
        }

        int64_t minEnergy{99999999999};
        for (auto i = 0UL; i < 1150UL; ++i)
        {
            const auto energy = ComputeEnergyPart2(positions, i);
            if (energy < minEnergy)
            {
                minEnergy = energy;
            }
        }

        std::cout << minEnergy << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
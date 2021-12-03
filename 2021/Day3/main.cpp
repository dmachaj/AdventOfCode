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
        std::vector<std::string> readings;
        std::string input;
        while (std::getline(std::cin, input))
        {
            readings.emplace_back(move(input));
        }

        uint32_t gamma{};
        uint32_t epsilon{};
        for (auto i = 0UL; i < readings[0].size(); ++i)
        {
            uint32_t zeros{};
            uint32_t ones{};
            for (const auto& reading : readings)
            {
                if (reading[i] == '1')
                {
                    ones++;
                }
                else
                {
                    ++zeros;
                }
            }

            if (ones > zeros)
            {
                gamma += 1 << (readings[0].size() - 1 - i);
            }
            else
            {
                epsilon += 1 << (readings[0].size() - 1 - i);
            }
        }

        std::cout << (gamma * epsilon) << std::endl;
    }

    std::pair<uint32_t, uint32_t> CountBits(const std::vector<std::string>& readings, uint32_t bitPosition)
    {
        uint32_t zeros{};
        uint32_t ones{};
        for (const auto& reading : readings)
        {
            if (reading[bitPosition] == '1')
            {
                ++ones;
            }
            else
            {
                ++zeros;
            }
        }

        return {zeros, ones};
    }

    uint32_t ComputeReading(const std::vector<std::string>& readings, bool isOxygen)
    {
        std::string finalReading;

        std::vector<std::string> readingsCopy(readings);
        for (auto i = 0UL; i < readings[0].size(); ++i)
        {
            if (readingsCopy.size() == 1)
            {
                finalReading = readingsCopy[0];
                break;
            }

            const auto bits = CountBits(readingsCopy, i);

            std::vector<std::string> readingsTemp;
            for (const auto& reading : readingsCopy)
            {
                const char tieWinner = isOxygen ? '1' : '0';
                const char oneIsMostCommonFilter = isOxygen ? '1' : '0';
                const char zeroIsMostCommonFilter = isOxygen ? '0' : '1';

                if (bits.second == bits.first && reading[i] == tieWinner)
                {
                    readingsTemp.emplace_back(reading);
                }
                else if (bits.second > bits.first && reading[i] == oneIsMostCommonFilter)
                {
                    readingsTemp.emplace_back(reading);
                }
                else if (bits.second < bits.first && reading[i] == zeroIsMostCommonFilter)
                {
                    readingsTemp.emplace_back(reading);
                }
            }
            std::swap(readingsCopy, readingsTemp);
        }

        if (finalReading.empty())
        {
            if (readingsCopy.size() != 1)
            {
                throw std::exception();
            }
            finalReading = readingsCopy[0];
        }
        
        uint32_t result{};
        for (auto i = 0UL; i < finalReading.size(); ++i)
        {
            if (finalReading[i] == '1')
            {
                result += 1 << (finalReading.size() - 1 - i);
            }
        }
        return result;
    }

    void Part2()
    {
        std::vector<std::string> readings;
        std::string input;
        while (std::getline(std::cin, input))
        {
            readings.emplace_back(move(input));
        }

        uint32_t oxygenResult = ComputeReading(readings, true);
        uint32_t co2Result = ComputeReading(readings, false);

        std::cerr << "Oxygen: " << oxygenResult << std::endl;
        std::cerr << "CO2: " << co2Result << std::endl;
        std::cout << (oxygenResult * co2Result) << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
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
        std::string input;
        std::getline(std::cin, input);
        uint32_t earliestTime = std::atoi(input.c_str());

        vector<uint32_t> busses{};
        std::string busInput;
        while (std::getline(std::cin, busInput, ','))
        {
            const auto busNum = std::atoi(busInput.c_str());
            if (busNum != 0)
            {
                busses.emplace_back(busNum);
            }
        }

        map<uint32_t, uint32_t> busToTime{};
        for (const auto bus : busses)
        {
            uint32_t time = bus;
            while (time < earliestTime)
            {
                time += bus;
            }
            busToTime[time] = bus;
        }

        const auto earliestBusIter = std::min_element(busToTime.begin(), busToTime.end());

        std::cout << (earliestBusIter->second * (earliestBusIter->first - earliestTime)) << std::endl;
    }

    void Part2()
    {
        // std::string throwAway;
        // std::getline(std::cin, throwAway);

        vector<uint32_t> busses{};
        std::string busInput;
        while (std::getline(std::cin, busInput, ','))
        {
            const auto busNum = std::atoi(busInput.c_str());
            busses.emplace_back(busNum); // 0 is NOOP placeholder
        }

        // bus number, offset from first bus
        vector<pair<uint32_t, uint32_t>> busOffsets{};
        for (auto i = 0UL; i < busses.size(); ++i)
        {
            if (busses[i] != 0)
            {
                busOffsets.emplace_back(make_pair(busses[i], i));
            }
        }

        uint64_t firstTwoSolution1{};
        {
            uint64_t counter{busOffsets[0].first};
            while(true)
            {
                if (((counter + busOffsets[1].second) % busOffsets[1].first) == 0)
                {
                    firstTwoSolution1 = counter;
                    break;
                }
                counter += busOffsets[0].first;
            }
        }

        uint64_t firstTwoSolution2{};
        {
            uint64_t counter{firstTwoSolution1 + busOffsets[0].first};
            while(true)
            {
                if (((counter + busOffsets[1].second) % busOffsets[1].first) == 0)
                {
                    firstTwoSolution2 = counter;
                    break;
                }
                counter += busOffsets[0].first;
            }
        }
        
        uint64_t firstTwoSolutionInterval = firstTwoSolution2 - firstTwoSolution1;
        uint64_t counter{firstTwoSolution1};
        while(true)
        {
            bool solved{true};
            for (const auto& bus : busOffsets)
            {
                if (((counter + bus.second) % bus.first) != 0)
                {
                    solved = false;
                    break;
                }
            }

            if (solved)
            {
                break;
            }
            counter += firstTwoSolutionInterval;
        }

        std::cout << counter << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
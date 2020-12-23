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

 //#define LOG_MOVES

namespace
{
    uint32_t FindDestination(const std::list<uint32_t>& cups, uint32_t currentCup, const std::list<uint32_t>& pickup, const uint32_t maxVal)
    {
        // If it isn't in the pickup list or below zero then it must be valid.  No need to search for inclusion.  All values are present.
        auto destination = currentCup - 1;
        while ((destination == 0) || (std::find(pickup.begin(), pickup.end(), destination) != pickup.end()))
        {
            if (destination == 0)
            {
                destination = maxVal;
            }
            else
            {
                --destination;
            }
        }
        return destination;
    }

    void RunSimulation(std::list<uint32_t>& cups, const uint32_t iterationCount, const uint32_t maxVal)
    {
        // To vastly increase speed we don't bother copying picked up cups from the source to the destination.
        // Rather, we move them into a map that tracks to the destination cup.  If we come across that destination
        // cup then we will quickly copy them into place so that nobody is the wiser.  At various points we
        // need to see if our desination cup is in one of these stashes, so we also keep a lookup table from a
        // stashed number to the destination that will receive it.
        std::unordered_map<uint32_t, std::list<uint32_t>> stashedDestinationCopy;
        std::unordered_map<uint32_t, uint32_t> stashedDestinationHost;

        const auto ApplyDeferredCopy = [](
            std::list<uint32_t>& cups, std::list<uint32_t>::iterator iter, 
            std::unordered_map<uint32_t, std::list<uint32_t>>& stashedDestinationCopy,
            std::unordered_map<uint32_t, uint32_t>& stashedDestinationHost)
        {
            if (stashedDestinationCopy.find(*iter) != stashedDestinationCopy.end())
            {
                auto iterCopy = iter;
                auto pickup = stashedDestinationCopy.find(*iter)->second;
                cups.insert(++iterCopy, pickup.begin(), pickup.end());
                stashedDestinationCopy.erase(*iter);
                for (auto item : pickup)
                {
                    stashedDestinationHost.erase(item);
                }
            }
        };

        auto iter = cups.begin();
        for (auto i = 0UL; i < iterationCount; ++i)
        {
            if ((i + 1) % 100000 == 0)
            {
                std::cerr << "Iteration count: " << (i + 1) << std::endl;
            }

            const uint32_t currentCup = *iter;
#ifdef LOG_MOVES
            const auto beginCupsCopy = cups;
#endif // LOG_MOVES

            ApplyDeferredCopy(cups, iter, stashedDestinationCopy, stashedDestinationHost);

            std::list<uint32_t> pickup;
            ++iter;
            if (iter == cups.end()) iter = cups.begin();
            ApplyDeferredCopy(cups, iter, stashedDestinationCopy, stashedDestinationHost);
            pickup.push_back(*iter);
            cups.erase(iter);
            ++iter;

            if (iter == cups.end()) iter = cups.begin();
            ApplyDeferredCopy(cups, iter, stashedDestinationCopy, stashedDestinationHost);
            pickup.push_back(*iter);
            cups.erase(iter);
            ++iter;

            if (iter == cups.end()) iter = cups.begin();
            ApplyDeferredCopy(cups, iter, stashedDestinationCopy, stashedDestinationHost);
            pickup.push_back(*iter);
            cups.erase(iter);
            ++iter;

            uint32_t destination = FindDestination(cups, currentCup, pickup, maxVal);

            if (stashedDestinationHost.find(destination) != stashedDestinationHost.end())
            {
                auto host = stashedDestinationHost[destination];
                auto host2 = stashedDestinationCopy[host];

                auto destIter = std::find(host2.begin(), host2.end(), destination);
                std::copy(pickup.begin(), pickup.end(), ++destIter);
            }

            for (auto item : pickup)
            {
                stashedDestinationHost[item] = destination;
            }

#ifdef LOG_MOVES
            stashedDestinationCopy[destination] = pickup;
#else
            stashedDestinationCopy[destination] = std::move(pickup);
#endif // LOG_MOVES

#ifdef LOG_MOVES
            std::cerr << "-- move " << (i + 1) << " --" << std::endl;
            std::cerr << "cups: ";
            for (auto cup : beginCupsCopy)
            {
                if (cup == currentCup)
                {
                    std::cerr << "(" << cup << ") ";
                }
                else
                {
                    std::cerr << cup << " ";
                }
            }
            std::cerr << std::endl;
            std::cerr << "pick up: ";
            for (auto cup : pickup)
            {
                std::cerr << cup << " ";
            }
            std::cerr << std::endl;
            std::cerr << "destination: " << destination << std::endl << std::endl;
#endif // LOG_MOVES

            // No need to find the next cup.  We're already pointing at it!
            if (iter == cups.end())
            {
                iter = cups.begin();
            }
        }

        // Apply pending FYI's before we complete.  Just loop through the list once for this.
        for (auto finalIter = cups.begin(); finalIter != cups.end(); ++finalIter)
        {
            ApplyDeferredCopy(cups, finalIter, stashedDestinationCopy, stashedDestinationHost);
        }
    }

    void Part1()
    {
        std::string input;
        std::getline(std::cin, input);
        
        std::list<uint32_t> cups;
        for (auto letter : input)
        {
            char temp[] = { letter, '\0' };
            cups.emplace_back((uint32_t)std::atoi(temp));
        }
        const uint32_t maxVal = *std::max_element(cups.begin(), cups.end());

        constexpr uint32_t c_iterationCount{100};
        RunSimulation(cups, c_iterationCount, maxVal);

        auto resultIter = std::find(cups.begin(), cups.end(), 1UL);
        std::ostringstream result;
        while (true)
        {
            ++resultIter;
            if (resultIter == cups.end())
            {
                resultIter = cups.begin();
            }
            
            if (*resultIter == 1) break;

            result << *resultIter;
        }

        std::cout << result.str() << std::endl;
    }

    void Part2()
    {
        std::string input;
        std::getline(std::cin, input);
        
        std::list<uint32_t> cups;
        for (auto letter : input)
        {
            char temp[] = { letter, '\0' };
            cups.emplace_back((uint32_t)std::atoi(temp));
        }
        const uint32_t maxVal = *std::max_element(cups.begin(), cups.end());

        constexpr uint32_t c_elementCount{ 1000000 };
        for (auto i = maxVal + 1; i <= c_elementCount; ++i)
        {
            cups.emplace_back(i);
        }

        constexpr uint32_t c_iterationCount{ 10000000 };
        RunSimulation(cups, c_iterationCount, c_elementCount);

        auto resultIter = std::find(cups.begin(), cups.end(), 1UL);
        ++resultIter;
        if (resultIter == cups.end())
        {
            resultIter = cups.begin();
        }
        uint64_t result = *resultIter;
        ++resultIter;
        if (resultIter == cups.end())
        {
            resultIter = cups.begin();
        }
        result *= *resultIter;

        std::cout << result << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
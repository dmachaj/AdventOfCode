#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
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

uint64_t ComputeWinners(uint64_t time, uint64_t distance)
{
    if (time == 0 && distance == 0) return 1;

    uint64_t result{};
    for (auto i = 1UL; i < time; ++i)
    {
        const auto moveTime = time - i;
        const auto moveSpeed = i;
        if ((moveTime * moveSpeed) > distance)
        {
            ++result;
        }
    }

    return result;
}

void Part1()
{
    std::string time;
    std::getline(std::cin, time);
    time = time.substr(time.find(':') + 1);
    std::istringstream timeStream(time);
    uint64_t time1, time2, time3, time4{};
    timeStream >> time1 >> time2 >> time3 >> time4;

    std::string distance;
    std::getline(std::cin, distance);
    distance = distance.substr(distance.find(':') + 1);
    std::istringstream distanceStream(distance);
    uint64_t dist1, dist2, dist3, dist4{};
    distanceStream >> dist1 >> dist2 >> dist3 >> dist4;

    const auto win1 = ComputeWinners(time1, dist1);
    const auto win2 = ComputeWinners(time2, dist2);
    const auto win3 = ComputeWinners(time3, dist3);
    const auto win4 = ComputeWinners(time4, dist4);

    std::cout << (win1 * win2 * win3 * win4) << std::endl;
}

void Part2()
{
    std::string time;
    std::getline(std::cin, time);
    time = time.substr(time.find(':') + 1);
    std::string timeStr;
    for (const char c : time)
    {
        if (c != ' ')
        {
            timeStr += c;
        }
    }
    const auto time1 = std::strtoull(timeStr.c_str(), nullptr, 10);

    std::string distance;
    std::getline(std::cin, distance);
    distance = distance.substr(distance.find(':') + 1);
    std::string distStr;
    for (const char c : distance)
    {
        if (c != ' ')
        {
            distStr += c;
        }
    }
    const auto dist1 = std::strtoull(distStr.c_str(), nullptr, 10);

    const auto win1 = ComputeWinners(time1, dist1);

    std::cout << win1 << std::endl;
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
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

namespace
{
    using point_t = std::pair<int, int>;

    void MarkCoords(std::unordered_set<int>& xCoords, point_t sensor, point_t beacon, int c_lineToWatch)
    {
        const auto distanceToBeacon = std::abs(sensor.first - beacon.first) + std::abs(sensor.second - beacon.second);
        const auto distanceToLine = std::abs(sensor.second - c_lineToWatch);

        // Early exit if we are nowhere near the line.
        if (distanceToLine > distanceToBeacon)
        {
            return;
        }

        auto xLeft = sensor.first;
        auto xRight = sensor.first;
        for (auto y = 0; y <= (distanceToBeacon - distanceToLine); ++y)
        {
            xCoords.emplace(xLeft);
            xCoords.emplace(xRight);

            --xLeft;
            ++xRight;
        }
    }

    void Part1()
    {
        std::unordered_set<int> xCoords;
        std::vector<point_t> invalidBeacons;

        std::string input;
        std::getline(std::cin, input);
        const auto c_lineToWatch = std::atoi(input.c_str());

        while (std::getline(std::cin, input))
        {
            auto firstX = input.find('x');
            auto comma = input.find(',', firstX);
            auto xStr = input.substr(firstX + 2, comma - firstX);
            auto x = std::atoi(xStr.c_str());

            auto firstY = input.find('y', firstX);
            comma = input.find(',', firstY);
            auto yStr = input.substr(firstY + 2);
            auto y = std::atoi(yStr.c_str());

            auto sensor = make_pair(x, y);

            auto secondX = input.find('x', firstY);
            comma = input.find(',', secondX);
            xStr = input.substr(secondX + 2, comma - secondX);
            x = std::atoi(xStr.c_str());

            auto secondY = input.find('y', secondX);
            comma = input.find(',', secondY);
            yStr = input.substr(secondY + 2);
            y = std::atoi(yStr.c_str());

            auto beacon = make_pair(x, y);

            MarkCoords(xCoords, sensor, beacon, c_lineToWatch);

            if (beacon.second == c_lineToWatch)
            {
                invalidBeacons.emplace_back(beacon);
            }
        }

        for (auto b : invalidBeacons)
        {
            if (xCoords.find(b.first) != xCoords.end())
            {
                xCoords.erase(b.first);
            }
        }

        std::cout << xCoords.size() << std::endl;
    }

    void MarkCoords2(std::unordered_set<int>& xCoords, point_t sensor, point_t beacon, int c_lineToWatch, int maxDimension)
    {
        const auto distanceToBeacon = std::abs(sensor.first - beacon.first) + std::abs(sensor.second - beacon.second);
        const auto distanceToLine = std::abs(sensor.second - c_lineToWatch);

        // Early exit if we are nowhere near the line.
        if (distanceToLine > distanceToBeacon)
        {
            return;
        }

        auto xLeft = sensor.first;
        auto xRight = sensor.first;
        for (auto y = 0; y <= (distanceToBeacon - distanceToLine); ++y)
        {
            if ((xLeft >= 0) && (xLeft <= maxDimension))
            {
                xCoords.emplace(xLeft);
            }

            if ((xRight >= 0) && (xRight <= maxDimension))
            {
                xCoords.emplace(xRight);
            }

            --xLeft;
            ++xRight;
        }
    }

    void Part2()
    {
        std::vector<std::pair<point_t, point_t>> combos;

        std::string input;
        std::getline(std::cin, input);
        const auto c_maxDimension = std::atoi(input.c_str());

        while (std::getline(std::cin, input))
        {
            auto firstX = input.find('x');
            auto comma = input.find(',', firstX);
            auto xStr = input.substr(firstX + 2, comma - firstX);
            auto x = std::atoi(xStr.c_str());

            auto firstY = input.find('y', firstX);
            comma = input.find(',', firstY);
            auto yStr = input.substr(firstY + 2);
            auto y = std::atoi(yStr.c_str());

            auto sensor = make_pair(x, y);

            auto secondX = input.find('x', firstY);
            comma = input.find(',', secondX);
            xStr = input.substr(secondX + 2, comma - secondX);
            x = std::atoi(xStr.c_str());

            auto secondY = input.find('y', secondX);
            comma = input.find(',', secondY);
            yStr = input.substr(secondY + 2);
            y = std::atoi(yStr.c_str());

            auto beacon = make_pair(x, y);

            combos.emplace_back(std::make_pair(std::move(sensor), std::move(beacon)));
        }

        point_t winner{};
        for (auto y = 0; y <= c_maxDimension; ++y)
        {
            if (y % 1000 == 0)
            {
                std::cerr << "Progress report.  y= " << y << std::endl;
            }
            std::unordered_set<int> xCoords;
            for (const auto& combo : combos)
            {
                MarkCoords2(xCoords, combo.first, combo.second, y, c_maxDimension);
            }

            if (xCoords.size() != (c_maxDimension + 1))
            {
                for (auto x = 0; x <= c_maxDimension; ++x)
                {
                    if (xCoords.find(x) == xCoords.end())
                    {
                        winner = make_pair(x, y);
                        break;
                    }
                }
                break;
            }
        }

        std::cout << (winner.first * 4000000 + winner.second) << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
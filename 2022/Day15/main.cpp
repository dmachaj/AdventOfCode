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
#include <optional>
#include <assert.h>
#include "HelpfulInfrastructure.h"

using namespace std; // I wouldn't normally do this
using namespace std::literals;

#pragma warning (disable: 4267) // size_t <-> uint
#pragma warning (disable: 4189) // unused variable
#pragma warning (disable: 4100) // unused param

namespace
{
    using point_t = std::pair<int64_t, int64_t>;

    void MarkCoords(std::unordered_set<int64_t>& xCoords, point_t sensor, point_t beacon, int64_t c_lineToWatch)
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
        std::unordered_set<int64_t> xCoords;
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

    bool IsWinner(const std::vector<std::pair<point_t, point_t>>& combos, int64_t x, int64_t y, int64_t c_maxDimension)
    {
        if ((x < 0) || (x > c_maxDimension))
        {
            return false;
        }

        if ((y < 0) || (y > c_maxDimension))
        {
            return false;
        }

        point_t winner{};

        for (const auto& combo : combos)
        {
            const auto& sensor = combo.first;
            const auto& beacon = combo.second;
            const auto distanceToBeacon = std::abs(sensor.first - beacon.first) + std::abs(sensor.second - beacon.second);

            const auto distanceToX = std::abs(sensor.first - x);
            const auto distanceToY = std::abs(sensor.second - y);
            if (distanceToX + distanceToY <= distanceToBeacon)
            {
                return false;
            }
        }

        return true;
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
        // Algorithm summary:
        //
        // Crawl the outer edge of each diamond.  The answer -must- be immediately outside the bounds of at least one
        // diamond and in reality several.  Quickly(ish) check that any given point along the way does not intersect
        // anything.  If it doesn't, it is the answer.
        for (const auto& combo : combos)
        {
            const auto& sensor = combo.first;
            const auto& beacon = combo.second;

            const auto distanceToBeacon = std::abs(sensor.first - beacon.first) + std::abs(sensor.second - beacon.second);

            auto top = sensor;
            top.second -= distanceToBeacon + 1;
            auto bottom = sensor;
            bottom.second += distanceToBeacon + 1;
            auto left = sensor;
            left.first -= distanceToBeacon + 1;
            auto right = sensor;
            right.first += distanceToBeacon + 1;

            for (auto x = left.first; x <= right.first; ++x)
            {
                if ((x < 0) || (x > c_maxDimension))
                {
                    continue;
                }

                auto yTop = left.second - (x - left.first);
                auto yBottom = left.second + (x - left.first);

                if (x == sensor.first)
                {
                    auto myTop = make_pair(x, yTop);
                    assert(top == myTop);
                    auto myBottom = make_pair(x, yBottom);
                    assert(bottom == myBottom);
                }

                if ((yTop >= 0) && (yTop < c_maxDimension))
                {
                    if (IsWinner(combos, x, yTop, c_maxDimension))
                    {
                        winner = make_pair(x, yTop);
                    }
                }

                if ((yBottom >= 0) && (yBottom < c_maxDimension))
                {
                    if (IsWinner(combos, x, yBottom, c_maxDimension))
                    {
                        winner = make_pair(x, yBottom);
                    }
                }
            }
        }

        if (winner.first == 0 && winner.second == 0)
        {
            std::cout << "Answer not found" << std::endl;
            return;
        }

        std::cout << (winner.first * 4000000 + winner.second) << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
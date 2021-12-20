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
    struct Target
    {
        int64_t xMin, xMax, yMin, yMax;
    };

    bool DoesHitTarget(const Target& target, int64_t dx, int64_t dy)
    {
        int64_t x{};
        int64_t y{};

        while (true)
        {
            if (x > target.xMax) return false;
            if (y < target.yMin) return false;

            if ((x >= target.xMin) && (x <= target.xMax) && (y >= target.yMin) && (y <= target.yMax)) return true;

            x += dx;
            y += dy;
            if (dx > 0) dx -= 1;
            else if (dx < 0) dx += 1;
            dy -= 1;
        }
    }

    Target ParseInput()
    {
        Target result{};

        std::string input;
        std::getline(std::cin, input);
        const auto xStr = input.substr(input.find('x'), input.find(',') - input.find('x'));
        const auto yStr = input.substr(input.find('y'));

        const auto xMinStr = xStr.substr(2, xStr.find('.') - 2);
        const auto xMaxStr = xStr.substr(xStr.find('.') + 2);

        const auto yMinStr = yStr.substr(2, yStr.find('.') - 2);
        const auto yMaxStr = yStr.substr(yStr.find('.') + 2);

        result.xMin = std::atoi(xMinStr.c_str());
        result.xMax = std::atoi(xMaxStr.c_str());
        result.yMin = std::atoi(yMinStr.c_str());
        result.yMax = std::atoi(yMaxStr.c_str());

        return result;
    }

    void Part1()
    {
        const auto target = ParseInput();

        uint64_t maxVelocity = std::abs(target.yMin) - 1ULL;
        uint64_t maxHeight{};
        while (maxVelocity > 0)
        {
            maxHeight += maxVelocity;
            maxVelocity -= 1;
        }

        std::cout << maxHeight << std::endl;
    }

    void Part2()
    {
        const auto target = ParseInput();

        std::unordered_set<uint64_t> xVelocities{};
        for (auto i = 1LL; i <= target.xMax; ++i)
        {
            auto position = 0LL;
            auto dx = i;
            while (true)
            {
                position += dx;
                --dx;

                if ((position >= target.xMin) && (position <= target.xMax))
                {
                    xVelocities.emplace(i);
                    break;
                }

                if (position > target.xMax)
                {
                    break;
                }

                if (dx == 0)
                {
                    break;
                }
            }
        }

        uint64_t result{};
        for (auto i = target.yMin; i < std::abs(target.yMin); ++i)
        {
            for (auto x : xVelocities)
            {
                if (DoesHitTarget(target, x, i))
                {
                    ++result;
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
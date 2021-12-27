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

using coord_t = std::tuple<int32_t, int32_t, int32_t>;

struct TupleHash
{
    size_t operator()(const coord_t& pt) const noexcept
    {
        auto h1 = std::hash<int64_t>{}(std::get<0>(pt));
        auto h2 = std::hash<int64_t>{}(std::get<1>(pt));
        auto h3 = std::hash<int64_t>{}(std::get<2>(pt));
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

using grid_t = std::unordered_map<coord_t, bool, TupleHash>;

namespace
{
    void Part1()
    {
        grid_t grid;

        std::string input;
        while (std::getline(std::cin, input))
        {
            const bool isOn = input[1] == 'n';

            const uint32_t xCharPos = input.find('x');
            const uint32_t yCharPos = input.find('y');
            const uint32_t zCharPos = input.find('z');

            const auto xStr = input.substr(xCharPos + 2, yCharPos - xCharPos - 3);
            const auto yStr = input.substr(yCharPos + 2, zCharPos - yCharPos - 3);
            const auto zStr = input.substr(zCharPos + 2);

            const int32_t xMin = std::atoi(xStr.c_str());
            const int32_t xMax = std::atoi(xStr.substr(xStr.find('.') + 2).c_str());
            
            const int32_t yMin = std::atoi(yStr.c_str());
            const int32_t yMax = std::atoi(yStr.substr(yStr.find('.') + 2).c_str());

            const int32_t zMin = std::atoi(zStr.c_str());
            const int32_t zMax = std::atoi(zStr.substr(zStr.find('.') + 2).c_str());

            for (int32_t x = std::max(-50, xMin); x <= std::min(50, xMax); ++x)
            {
                for (int32_t y = std::max(-50, yMin); y <= std::min(50, yMax); ++y)
                {
                    for (int32_t z = std::max(-50, zMin); z <= std::min(50, zMax); ++z)
                    {
                        grid[make_tuple(x, y, z)] = isOn;
                    }
                }
            }
        }

        const auto result = std::accumulate(grid.begin(), grid.end(), 0UL, [](auto sum, const std::pair<coord_t, bool>& pt)
        {
            return (pt.second) ? sum + 1 : sum;
        });
        std::cout << result << std::endl;
    }

    void Part2()
    {
        std::string input;
        int32_t minVal{0xFFFFF};
        int32_t maxVal{0};
        while (std::getline(std::cin, input))
        {
            const bool isOn = input[1] == 'n';

            const uint32_t xCharPos = input.find('x');
            const uint32_t yCharPos = input.find('y');
            const uint32_t zCharPos = input.find('z');

            const auto xStr = input.substr(xCharPos + 2, yCharPos - xCharPos - 3);
            const auto yStr = input.substr(yCharPos + 2, zCharPos - yCharPos - 3);
            const auto zStr = input.substr(zCharPos + 2);

            const int32_t xMin = std::atoi(xStr.c_str());
            const int32_t xMax = std::atoi(xStr.substr(xStr.find('.') + 2).c_str());
            
            const int32_t yMin = std::atoi(yStr.c_str());
            const int32_t yMax = std::atoi(yStr.substr(yStr.find('.') + 2).c_str());

            const int32_t zMin = std::atoi(zStr.c_str());
            const int32_t zMax = std::atoi(zStr.substr(zStr.find('.') + 2).c_str());

            minVal = std::min(xMin, minVal);
            minVal = std::min(yMin, minVal);
            minVal = std::min(zMin, minVal);

            maxVal = std::max(xMax, maxVal);
            maxVal = std::max(yMax, maxVal);
            maxVal = std::max(zMax, maxVal);
        }

        std::cerr << "Min: " << minVal << std::endl << "Max: " << maxVal << std::endl;
        std::cout << 0 << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
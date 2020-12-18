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

namespace
{
    struct Point3D
    {
        Point3D(int64_t x, int64_t y, int64_t z):
            x(x), y(y), z(z)
        {
        }
        int64_t x{};
        int64_t y{};
        int64_t z{};
        
    };

    bool operator==(const Point3D& lhs, const Point3D& rhs)
    {
        return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
    }

    struct Point3DHash
    {
        size_t operator()(const Point3D& pt) const noexcept
        {
            auto h1 = std::hash<int64_t>{}(pt.x);
            auto h2 = std::hash<int64_t>{}(pt.y);
            auto h3 = std::hash<int64_t>{}(pt.z);
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };

    struct Space
    {
        bool isActive{false};
        uint32_t nearbyActives{};

        void UpdateActivation()
        {
            if (isActive && ((nearbyActives < 2) || (nearbyActives > 3)))
            {
                isActive = false;
            }
            else if (!isActive && (nearbyActives == 3))
            {
                isActive = true;
            }
            nearbyActives = 0;
        }
    };

    void UpdateActiveNeighbors(unordered_map<Point3D, Space, Point3DHash>& allPoints, const Point3D& pt)
    {
        // if (!allPoints[pt].isActive) throw 0;

        uint32_t result{};
        for (auto x = -1; x <= 1; ++x)
        {
            for (auto y = -1; y <= 1; ++y)
            {
                for (auto z = -1; z <= 1; ++z)
                {
                    if (x == 0 && y == 0 && z == 0) continue;
                    Point3D lookup(pt.x + x, pt.y + y, pt.z + z);
                    auto& space = allPoints[lookup];
                    space.nearbyActives++; // also allocates!
                }
            }
        }
    }

    void Part1()
    {
        unordered_map<Point3D, Space, Point3DHash> allPoints{};
        uint32_t width{};
        uint32_t height{};
        std::string input;
        while (std::getline(std::cin, input))
        {
            width = input.length();
            for (auto x = 0UL; x < width; ++x)
            {
                const bool isActive = input[x] == '#';
                const Point3D pt{x, height, 0};
                allPoints[pt].isActive = isActive; // also allocates!
            }
            ++height;
        }

        for (auto i = 0UL; i < 6; ++i)
        {
            std::vector<Point3D> pointsCopy;
            for (auto& pt : allPoints)
            {
                if (pt.second.isActive)
                {
                    pointsCopy.emplace_back(pt.first);
                }
            }

            for (auto& pt : pointsCopy)
            {
                UpdateActiveNeighbors(allPoints, pt);
            }

            for (auto& pt : allPoints)
            {
                pt.second.UpdateActivation();
            }
        }

        const auto result = std::accumulate(allPoints.begin(), allPoints.end(), 0UL, [](const uint32_t sum, const auto& point)
        {
            return point.second.isActive ? (sum + 1) : sum;
        });
        std::cout << result << std::endl;
    }

    void Part2()
    {
        std::cout << 0 << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
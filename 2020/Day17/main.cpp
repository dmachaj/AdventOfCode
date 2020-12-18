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
        auto& currentSpace = allPoints[pt];
        if (!currentSpace.isActive) throw 0;

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

    struct Point4D
    {
        Point4D(int64_t x, int64_t y, int64_t z, int64_t w):
            x(x), y(y), z(z), w(w)
        {
        }
        int64_t x{};
        int64_t y{};
        int64_t z{};
        int64_t w{};
    };

    bool operator==(const Point4D& lhs, const Point4D& rhs)
    {
        return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z) && (lhs.w == rhs.w);
    }

    struct Point4DHash
    {
        size_t operator()(const Point4D& pt) const noexcept
        {
            auto h1 = std::hash<int64_t>{}(pt.x);
            auto h2 = std::hash<int64_t>{}(pt.y);
            auto h3 = std::hash<int64_t>{}(pt.z);
            auto h4 = std::hash<int64_t>{}(pt.w);
            return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
        }
    };
    
    void UpdateActiveNeighborsPart2(unordered_map<Point4D, Space, Point4DHash>& allPoints, const Point4D& pt)
    {
        auto& currentSpace = allPoints[pt];
        if (!currentSpace.isActive) throw 0;

        for (auto x = -1; x <= 1; ++x)
        {
            for (auto y = -1; y <= 1; ++y)
            {
                for (auto z = -1; z <= 1; ++z)
                {
                    for (auto w = -1; w <= 1; ++w)
                    {
                        if (x == 0 && y == 0 && z == 0 && w == 0) continue;
                        Point4D lookup(pt.x + x, pt.y + y, pt.z + z, pt.w + w);
                        auto& space = allPoints[lookup];
                        space.nearbyActives++; // also allocates!
                    }
                }
            }
        }
    }

    void Part2()
    {
        unordered_map<Point4D, Space, Point4DHash> allPoints{};
        uint32_t width{};
        uint32_t height{};
        std::string input;
        while (std::getline(std::cin, input))
        {
            width = input.length();
            for (auto x = 0UL; x < width; ++x)
            {
                const bool isActive = input[x] == '#';
                const Point4D pt{x, height, 0, 0};
                allPoints[pt].isActive = isActive; // also allocates!
            }
            ++height;
        }

        for (auto i = 0UL; i < 6; ++i)
        {
            std::vector<Point4D> pointsCopy;
            for (auto& pt : allPoints)
            {
                if (pt.second.isActive)
                {
                    pointsCopy.emplace_back(pt.first);
                }
            }

            for (auto& pt : pointsCopy)
            {
                UpdateActiveNeighborsPart2(allPoints, pt);
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
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
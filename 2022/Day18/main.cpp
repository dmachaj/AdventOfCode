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
    struct Point
    {
        Point(int32_t x, int32_t y, int32_t z):
            x(x), y(y), z(z)
        {}

        Point(const std::string& input)
        {
            const auto firstComma = input.find(',');
            const auto secondComma = input.find(',', firstComma + 1);

            auto xStr = input.substr(0, firstComma);
            x = std::atoi(xStr.c_str());

            auto yStr = input.substr(firstComma + 1, secondComma - firstComma);
            y = std::atoi(yStr.c_str());

            auto zStr = input.substr(secondComma + 1);
            z = std::atoi(zStr.c_str());
        }

        bool operator==(const Point& other) const
        {
            return (other.x == x) && (other.y == y) && (other.z == z);
        }

        std::vector<Point> Permute() const
        {
            std::vector<Point> result;

            result.emplace_back(Point{x - 1, y, z});
            result.emplace_back(Point{x + 1, y, z});
            result.emplace_back(Point{x, y - 1, z});
            result.emplace_back(Point{x, y + 1, z});
            result.emplace_back(Point{x, y, z - 1});
            result.emplace_back(Point{x, y, z + 1});

            return result;
        }

        int32_t x{};
        int32_t y{};
        int32_t z{};
    };

    struct PointHash
    {
        size_t operator()(const Point& pt) const noexcept
        {
            auto h1 = std::hash<int32_t>{}(pt.x);
            auto h2 = std::hash<int32_t>{}(pt.y);
            auto h3 = std::hash<int32_t>{}(pt.z);
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };

    using PointSet = std::unordered_set<Point, PointHash>;

    void Part1()
    {
        PointSet points;

        std::string input;
        while (std::getline(std::cin, input))
        {
            points.emplace(Point{input});
        }

        int32_t result{};
        for (const auto& point : points)
        {
            auto neighbors = point.Permute();
            for (const auto& n : neighbors)
            {
                if (points.find(n) == points.end())
                {
                    ++result;
                }
            }
        }

        std::cout << result << std::endl;
    }

    constexpr int32_t c_minDimension = -5;
    constexpr int32_t c_maxDimension = 25;

    void FindEdgesRecursive(const PointSet& points, PointSet& empty, int32_t x, int32_t y, int32_t z)
    {
        if (x < c_minDimension || x > c_maxDimension || y < c_minDimension || y > c_maxDimension || z < c_minDimension || z > c_maxDimension)
        {
            return;
        }

        const Point curr{x, y, z};
        if (points.find(curr) != points.end()) return; // This is solid, stop checking
        if (empty.find(curr) != empty.end()) return; // We've already been here, skip

        empty.emplace(std::move(curr));

        FindEdgesRecursive(points, empty, x + 1, y, z);
        FindEdgesRecursive(points, empty, x - 1, y, z);
        FindEdgesRecursive(points, empty, x, y + 1, z);
        FindEdgesRecursive(points, empty, x, y - 1, z);
        FindEdgesRecursive(points, empty, x, y, z + 1);
        FindEdgesRecursive(points, empty, x, y, z - 1);
    }

    void Part2()
    {
        PointSet points;

        std::string input;
        while (std::getline(std::cin, input))
        {
            points.emplace(Point{input});
        }

        // Compute and print the dimensions of the data
        // const auto maxX = std::max_element(points.begin(), points.end(), [](const Point& left, const Point& right) { return left.x < right.x; })->x;
        // const auto maxY = std::max_element(points.begin(), points.end(), [](const Point& left, const Point& right) { return left.y < right.y; })->y;
        // const auto maxZ = std::max_element(points.begin(), points.end(), [](const Point& left, const Point& right) { return left.z < right.z; })->z;
        // std::cerr << maxX << " " << maxY << " " << maxZ << std::endl;

        // Algorithm - Start at a low number and crawl all edges to find the outer surface of the lava droplet.
        //             Then (inefficiently) find the spaces that are in neither set which will be the internal
        //             air gaps.  Treat those as lava and then compute like Part1.
        PointSet empty;
        FindEdgesRecursive(points, empty, c_minDimension, c_minDimension, c_minDimension);

        for (auto x = c_minDimension; x <= c_maxDimension; ++x)
        {
            for (auto y = c_minDimension; y <= c_maxDimension; ++y)
            {
                for (auto z = c_minDimension; z <= c_maxDimension; ++z)
                {
                    const Point current{x, y, z};
                    if ((points.find(current) == points.end()) && (empty.find(current) == empty.end()))
                    {
                        points.emplace(std::move(current));
                    }
                }
            }
        }

        int32_t result{};
        for (const auto& point : points)
        {
            auto neighbors = point.Permute();
            for (const auto& n : neighbors)
            {
                if (points.find(n) == points.end())
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
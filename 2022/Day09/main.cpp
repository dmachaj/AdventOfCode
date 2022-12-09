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
    using point_t = std::pair<int32_t, int32_t>;
    struct PointHash
    {
        size_t operator()(const point_t& pt) const noexcept
        {
            auto h1 = std::hash<int32_t>{}(pt.first);
            auto h2 = std::hash<int32_t>{}(pt.second);
            return h1 ^ (h2 << 1);
        }
    };

    void PrintBoard(const std::unordered_set<point_t, PointHash>& visited, int32_t dimensions)
    {
        // for (auto y = dimensions; y >= -dimensions; --y)
        // {
        //     for (auto x = -dimensions; x <= dimensions; ++x)
        //     {
        //         if (x == 0 && y == 0)
        //         {
        //             cerr << "s";
        //             continue;
        //         }
                
        //         point_t curr{x, y};
        //         if (visited.find(curr) != visited.end())
        //         {
        //             std::cerr << "X";
        //         }
        //         else { std::cerr << "."; }
        //     }
        //     std::cerr << endl;
        // }
    }

    void Follow(point_t& head, point_t& tail)
    {
        auto diffX = head.first - tail.first;
        auto diffY = head.second - tail.second;

        while (abs(diffX) > 1 || abs(diffY) > 1)
        {
            int32_t adjustmentX, adjustmentY;
            if ((abs(diffX) != 0) && (abs(diffY != 0)))
            {
                adjustmentX = (diffX > 0) ? 1 : -1;
                adjustmentY = (diffY > 0) ? 1 : -1;
            }
            else if (abs(diffX != 0))
            {
                adjustmentX = (diffX > 0) ? 1 : -1;
                adjustmentY = 0;
            }
            else
            {
                adjustmentX = 0;
                adjustmentY = (diffY > 0) ? 1 : -1;
            }

            tail.first += adjustmentX;
            tail.second += adjustmentY;
            
            diffX = head.first - tail.first;
            diffY = head.second - tail.second;
        }
    }

    void Part1()
    {
        point_t head{};
        point_t tail{};

        std::unordered_set<point_t, PointHash> visited;
        visited.emplace(tail);

        std::string input;
        while (std::getline(std::cin, input))
        {
            auto direction = input[0];
            auto amount = std::atoi(input.substr(input.find(' ') + 1).c_str());

            int dx = 0;
            if (direction == 'R') dx = 1;
            if (direction == 'L') dx = -1;

            int dy = 0;
            if (direction == 'U') dy = 1;
            if (direction == 'D') dy = -1;

            for (auto i = 0; i < amount; ++i)
            {
                head.first += dx;
                head.second += dy;

                Follow(head, tail);

                visited.emplace(tail);
            }
        }

        PrintBoard(visited, 5);
        std::cout << visited.size() << std::endl;
    }

    void Part2()
    {
        constexpr uint32_t c_count{10UL};
        std::vector<point_t> knots(c_count, point_t{});

        std::unordered_set<point_t, PointHash> visited;
        visited.emplace(knots[9]);

        std::string input;
        while (std::getline(std::cin, input))
        {
            auto direction = input[0];
            auto amount = std::atoi(input.substr(input.find(' ') + 1).c_str());

            int dx = 0;
            if (direction == 'R') dx = 1;
            if (direction == 'L') dx = -1;

            int dy = 0;
            if (direction == 'U') dy = 1;
            if (direction == 'D') dy = -1;

            for (auto counter = 0; counter < amount; ++counter)
            {
                knots[0].first += dx;
                knots[0].second += dy;

                for (auto i = 1; i < c_count; ++i)
                {
                    auto& head = knots[i - 1];
                    auto& tail = knots[i];
                    Follow(head, tail);
                }

                visited.emplace(knots[c_count - 1]);
            }
        }

        PrintBoard(visited, 11);
        std::cout << visited.size() << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
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
    void Part1()
    {
        uint32_t width{};
        uint32_t height{};
        std::vector<uint32_t> map;

        {
            std::string input;
            while (std::getline(std::cin, input))
            {
                ++height;
                width = input.size();
                for (auto c : input)
                {
                    map.emplace_back(c - '0');
                }
            }
        }

        std::vector<uint32_t> allCosts(map.size(), 0);
        std::vector<tuple<uint32_t, uint32_t, uint32_t>> priorities;
        priorities.emplace_back(make_tuple(0, 0, 1));
        while (!priorities.empty())
        {
            auto minIter = std::min_element(priorities.begin(), priorities.end(),
                [](auto first, auto second)
                {
                    return std::get<2>(first) < std::get<2>(second);
                });
            auto x = std::get<0>(*minIter);
            auto y = std::get<1>(*minIter);
            auto cost = std::get<2>(*minIter);
            priorities.erase(minIter);

            allCosts[y * width + x] = cost;

            if ((x - 1) < width)
            {
                if (allCosts[y * width + (x - 1)] == 0)
                {
                    if (std::find_if(priorities.begin(), priorities.end(), [x, y](auto var)
                    {
                        return (std::get<0>(var) == x - 1) && (std::get<1>(var) == y);
                    }) == priorities.end())
                    {
                        const auto added = map[y * width + (x - 1)];
                        priorities.emplace_back(make_tuple(x - 1, y, cost + added));
                    }
                }
            }
            
            if ((x + 1) < width)
            {
                if (allCosts[y * width + (x + 1)] == 0)
                {
                    if (std::find_if(priorities.begin(), priorities.end(), [x, y](auto var)
                    {
                        return (std::get<0>(var) == x + 1) && (std::get<1>(var) == y);
                    }) == priorities.end())
                    {
                        const auto added = map[y * width + (x + 1)];
                        priorities.emplace_back(make_tuple(x + 1, y, cost + added));
                    }
                }
            }

            if ((y - 1) < height)
            {
                if (allCosts[(y - 1) * width + x] == 0)
                {
                    if (std::find_if(priorities.begin(), priorities.end(), [x, y](auto var)
                    {
                        return (std::get<0>(var) == x) && (std::get<1>(var) == y - 1);
                    }) == priorities.end())
                    {
                        const auto added = map[(y - 1) * width + x];
                        priorities.emplace_back(make_tuple(x, y - 1, cost + added));
                    }
                }
            }
            
            if ((y + 1) < height)
            {
                if (allCosts[(y + 1) * width + x] == 0)
                {
                    if (std::find_if(priorities.begin(), priorities.end(), [x, y](auto var)
                    {
                        return (std::get<0>(var) == x) && (std::get<1>(var) == y + 1);
                    }) == priorities.end())
                    {
                        const auto added = map[(y + 1) * width + x];
                        priorities.emplace_back(make_tuple(x, y + 1, cost + added));
                    }
                }
            }
        }

        std::cout << (allCosts[allCosts.size() - 1] - 1) << std::endl;
    }

    void Part2()
    {
        uint32_t width{};
        uint32_t height{};
        std::vector<uint32_t> map1;

        {
            std::string input;
            while (std::getline(std::cin, input))
            {
                ++height;
                width = input.size();
                for (auto c : input)
                {
                    map1.emplace_back(c - '0');
                }
            }
        }

        // Difference from part 1 : 5x the board with crazy logic.  
        std::vector<uint32_t> map(map1.size() * 25, 0);
        for (auto y = 0UL; y < height; ++y)
        {
            for (auto x = 0UL; x < width; ++x)
            {
                for (auto y2 = 0UL; y2 < 5; ++y2)
                {
                    for (auto x2 = 0UL; x2 < 5; ++x2)
                    {
                        auto yMult = y + (y2 * height);
                        auto xMult = x + (x2 * width);

                        auto newValue = map1[y * width + x];
                        newValue += y2;
                        newValue += x2;
                        if (newValue > 9) newValue -= 9;

                        map[yMult * (width * 5) + xMult] = newValue;
                    }
                }
            }
        }

        // We've now 5x'ed the board so 5x our size limits
        width *= 5;
        height *= 5;

        // for (auto y = 0UL; y < height; ++y)
        // {
        //     for (auto x = 0UL; x < width; ++x)
        //     {
        //         std::cerr << map[y * width + x];
        //     }
        //     std::cerr << std::endl;
        // }

        std::vector<uint32_t> allCosts(map.size(), 0);
        std::vector<tuple<uint32_t, uint32_t, uint32_t>> priorities;
        priorities.emplace_back(make_tuple(0, 0, 1));
        while (!priorities.empty())
        {
            auto minIter = std::min_element(priorities.begin(), priorities.end(),
                [](auto first, auto second)
                {
                    return std::get<2>(first) < std::get<2>(second);
                });
            auto x = std::get<0>(*minIter);
            auto y = std::get<1>(*minIter);
            auto cost = std::get<2>(*minIter);
            priorities.erase(minIter);

            allCosts[y * width + x] = cost;

            if ((x - 1) < width)
            {
                if (allCosts[y * width + (x - 1)] == 0)
                {
                    if (std::find_if(priorities.begin(), priorities.end(), [x, y](auto var)
                    {
                        return (std::get<0>(var) == x - 1) && (std::get<1>(var) == y);
                    }) == priorities.end())
                    {
                        const auto added = map[y * width + (x - 1)];
                        priorities.emplace_back(make_tuple(x - 1, y, cost + added));
                    }
                }
            }
            
            if ((x + 1) < width)
            {
                if (allCosts[y * width + (x + 1)] == 0)
                {
                    if (std::find_if(priorities.begin(), priorities.end(), [x, y](auto var)
                    {
                        return (std::get<0>(var) == x + 1) && (std::get<1>(var) == y);
                    }) == priorities.end())
                    {
                        const auto added = map[y * width + (x + 1)];
                        priorities.emplace_back(make_tuple(x + 1, y, cost + added));
                    }
                }
            }

            if ((y - 1) < height)
            {
                if (allCosts[(y - 1) * width + x] == 0)
                {
                    if (std::find_if(priorities.begin(), priorities.end(), [x, y](auto var)
                    {
                        return (std::get<0>(var) == x) && (std::get<1>(var) == y - 1);
                    }) == priorities.end())
                    {
                        const auto added = map[(y - 1) * width + x];
                        priorities.emplace_back(make_tuple(x, y - 1, cost + added));
                    }
                }
            }
            
            if ((y + 1) < height)
            {
                if (allCosts[(y + 1) * width + x] == 0)
                {
                    if (std::find_if(priorities.begin(), priorities.end(), [x, y](auto var)
                    {
                        return (std::get<0>(var) == x) && (std::get<1>(var) == y + 1);
                    }) == priorities.end())
                    {
                        const auto added = map[(y + 1) * width + x];
                        priorities.emplace_back(make_tuple(x, y + 1, cost + added));
                    }
                }
            }
        }

        std::cout << (allCosts[allCosts.size() - 1] - 1) << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
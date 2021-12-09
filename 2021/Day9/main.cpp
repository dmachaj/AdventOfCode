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
    bool IsLocalMin(const std::vector<int32_t>& map, int32_t location, uint32_t width)
    {
        if ((location % width) != 0) // not left edge
        {
            if (map[location - 1] <= map[location]) return false;
        }
        
        if ((location + 1) % width != 0) // not right edge
        {
            if (map[location + 1] <= map[location]) return false;
        }

        if (location >= (int32_t)width) // not top
        {
            if (map[location - width] <= map[location]) return false;
        }
        
        if ((location + width) <= map.size()) // not bottom
        {
            if (map[location + width] <= map[location]) return false;
        }

        return true;
    }

    void Part1()
    {
        std::vector<int32_t> map;
        uint32_t width{};
        std::string input;
        while (std::getline(std::cin, input))
        {
            for (const auto c : input)
            {
                std::string temp;
                temp += c;
                map.emplace_back(std::atoi(temp.c_str()));
            }
            width = input.size();
        }

        uint32_t total{};
        for (auto i = 0UL; i < map.size(); ++i)
        {
            if (IsLocalMin(map, i, width))
            {
                total += map[i] + 1;
            }
        }

        std::cout << total << std::endl;
    }

    uint32_t VisitBasin(const std::vector<int32_t>& map, uint32_t width, std::vector<bool>& visited, int32_t location)
    {
        uint32_t total{};

        if (visited[location]) return 0;

        visited[location] = true;
        if (map[location] == 9) return 0;

        if ((location % width) != 0) // not left edge
        {
            if (!visited[location - 1])
            {
                total += VisitBasin(map, width, visited, location - 1);
            }
        }
        
        if ((location + 1) % width != 0) // not right edge
        {
            if (!visited[location + 1])
            {
                total += VisitBasin(map, width, visited, location + 1);
            }
        }

        if (location >= (int32_t)width) // not top
        {
            if (!visited[location - width])
            {
                total += VisitBasin(map, width, visited, location - width);
            }
        }
        
        if ((location + width) <= map.size()) // not bottom
        {
            if (!visited[location + width])
            {
                total += VisitBasin(map, width, visited, location + width);
            }
        }

        return total + 1;
    }

    void Part2()
    {
        std::vector<int32_t> map;
        uint32_t width{};
        std::string input;
        while (std::getline(std::cin, input))
        {
            for (const auto c : input)
            {
                std::string temp;
                temp += c;
                map.emplace_back(std::atoi(temp.c_str()));
            }
            width = input.size();
        }

        std::vector<bool> visited;
        for (auto i = 0UL; i < map.size(); ++i)
        {
            visited.emplace_back(false);
        }

        std::vector<int32_t> basins;
        for (auto i = 0UL; i < map.size(); ++i)
        {
            if (!visited[i] && map[i] != 9)
            {
                basins.emplace_back(VisitBasin(map, width, visited, i));
            }
        }

        std::sort(basins.begin(), basins.end());
        uint32_t total{1};
        for (auto i = 0; i < 3; ++i)
        {
            total *= *(basins.rbegin() + i);
        }

        std::cout << total << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
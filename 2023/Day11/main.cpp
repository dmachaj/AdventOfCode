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

void Part1()
{
    std::string map;
    int width{};
    int height{};

    std::string input;
    while (std::getline(std::cin, input))
    {
        width = input.size();
        map.append(std::move(input));
        ++height;
    }

    // expand by row
    {
        int blankRows{};
        for (auto y = 0; y < height; ++y)
        {
            auto lineIter = map.begin() + (y * width);
            bool isEmpty = std::all_of(lineIter, lineIter + width, [](char c) { return c != '#'; });
            if (isEmpty)
            {
                ++blankRows;
                ++height;
                map.insert(lineIter, width, '.');
                ++y; // skip newly inserted row
            }
        }
        std::cerr << blankRows << " blank rows" << std::endl;
    }

    // expand by col
    {
        int blankCols{};
        for (auto x = 0; x < width; ++x)
        {
            bool isEmpty{true};
            for (auto iter = map.begin() + x; iter < map.end(); iter += width)
            {
                if (*iter == '#')
                {
                    isEmpty = false;
                    break;
                }
            }
            
            if (isEmpty)
            {
                ++blankCols;
                for (auto y = height - 1; y >= 0; --y)
                {
                    map.insert(map.begin() + (y * width + x), '.');
                }
                ++width;
                ++x; // skip newly inserted row
            }
        }
        std::cerr << blankCols << " blank columns" << std::endl;
    }

    std::vector<int> indexes;
    for (auto i = 0; i < map.size(); ++i)
    {
        if (map[i] == '#') indexes.emplace_back(i);
    }

    uint64_t result{};
    for (auto i = 0; i < indexes.size(); ++i)
    {
        const auto first = indexes[i];
        const auto x1 = first % width;
        const auto y1 = first / width;

        for (auto j = i + 1; j < indexes.size(); ++j)
        {
            if (i == j) continue;

            const auto second = indexes[j];
            const auto x2 = second % width;
            const auto y2 = second / width;

            const auto dist = (std::abs(x2 - x1) + std::abs(y2 - y1));
            result += dist;
        }
    }

    // for (auto y = 0; y < height; ++y)
    // {
    //     for (auto x = 0; x < width; ++x)
    //     {
    //         std::cerr << map[y * width + x];
    //     }
    //     std::cerr << "\n";
    // }

    std::cout << result << std::endl;
}

void Part2()
{
    std::string map;
    int64_t width{};
    int64_t height{};

    std::string input;
    std::getline(std::cin, input);
    const uint64_t c_growthRate = std::strtoull(input.c_str(), nullptr, 10);

    while (std::getline(std::cin, input))
    {
        width = input.size();
        map.append(std::move(input));
        ++height;
    }

    // expand by row
    std::vector<int64_t> blankRows;
    {
        for (auto y = 0; y < height; ++y)
        {
            auto lineIter = map.begin() + (y * width);
            bool isEmpty = std::all_of(lineIter, lineIter + width, [](char c) { return c != '#'; });
            if (isEmpty)
            {
                blankRows.emplace_back(y);
            }
        }
        std::cerr << blankRows.size() << " blank rows" << std::endl;
    }

    // expand by col
    std::vector<int64_t> blankCols;
    {
        for (auto x = 0; x < width; ++x)
        {
            bool isEmpty{true};
            for (auto iter = map.begin() + x; iter < map.end(); iter += width)
            {
                if (*iter == '#')
                {
                    isEmpty = false;
                    break;
                }
            }
            
            if (isEmpty)
            {
                blankCols.emplace_back(x);
            }
        }
        std::cerr << blankCols.size() << " blank columns" << std::endl;
    }

    std::vector<int64_t> indexes;
    for (auto i = 0; i < map.size(); ++i)
    {
        if (map[i] == '#') indexes.emplace_back(i);
    }

    uint64_t result{};
    for (auto i = 0; i < indexes.size(); ++i)
    {
        const auto first = indexes[i];
        const auto x1 = first % width;
        const auto y1 = first / width;

        for (auto j = i + 1; j < indexes.size(); ++j)
        {
            if (i == j) continue;

            const auto second = indexes[j];
            const auto x2 = second % width;
            const auto y2 = second / width;

            auto dist = (std::abs(x2 - x1) + std::abs(y2 - y1));

            for (auto row : blankRows)
            {
                const auto minY = std::min(y1, y2);
                const auto maxY = std::max(y1, y2);
                if (row > minY && row < maxY)
                {
                    dist += c_growthRate - 1;
                }
            }

            for (auto col : blankCols)
            {
                const auto minX = std::min(x1, x2);
                const auto maxX = std::max(x1, x2);
                if (col > minX && col < maxX)
                {
                    dist += c_growthRate - 1;
                }
            }

            result += dist;
        }
    }

    // for (auto y = 0; y < height; ++y)
    // {
    //     for (auto x = 0; x < width; ++x)
    //     {
    //         std::cerr << map[y * width + x];
    //     }
    //     std::cerr << "\n";
    // }

    std::cout << result << std::endl;
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
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

#pragma warning (disable: 4018) // '<': signed/unsigned mismatch
#pragma warning (disable: 4267) // size_t <-> uint
#pragma warning (disable: 4189) // unused variable
#pragma warning (disable: 4100) // unused param

namespace
{
    bool Trace(const vector<uint32_t>& map, uint32_t width, uint32_t height, uint32_t startX, uint32_t startY, int dx, int dy)
    {
        int x{(int)startX + dx};
        int y{(int)startY + dy};
        uint32_t startHeight{map[startY * width + startX]};

        while ((x >= 0 && x < width) && (y >= 0 && y < height))
        {
            if (map[y * width + x] >= startHeight)
            {
                return false;
            }

            x += dx;
            y += dy;
        }

        return true;
    }

    void Part1()
    {
        vector<uint32_t> map;
        uint32_t width{};

        std::string input;
        while (std::getline(std::cin, input))
        {
            width = input.length();
            for (auto i = 0; i < input.length(); ++i)
            {
                map.emplace_back(atoi(input.substr(i, 1).c_str()));
            }
        }

        const auto height = map.size() / width;

        vector<bool> visible(map.size(), false);
        for (auto x = 0UL; x < width; ++x)
        {
            for (auto y = 0UL; y < height; ++y)
            {
                if (x == 0 || x == width - 1 || y == 0 || y == height - 1)
                {
                    visible[y * width + x] = true;
                }
                else
                {
                    if (Trace(map, width, height, x, y, -1, 0) || Trace(map, width, height, x, y, 1, 0) || Trace(map, width, height, x, y, 0, -1) || Trace(map, width, height, x, y, 0, 1))
                    {
                        visible[y * width + x] = true;
                    }
                }
            }
        }

        // for (auto y = 0UL; y < height; ++y)
        // {
        //     for (auto x = 0UL; x < width; ++x)
        //     {
        //         std::cerr << (visible[y * width + x] ? "X" : " ");
        //     }
        //     std::cerr << std::endl;
        // }

        uint32_t result{};
        std::cout << std::accumulate(visible.begin(), visible.end(), result) << std::endl;
    }

    uint32_t Trace2(const vector<uint32_t>& map, uint32_t width, uint32_t height, uint32_t startX, uint32_t startY, int dx, int dy)
    {
        int x{(int)startX};
        int y{(int)startY};
        uint32_t startHeight{map[startY * width + startX]};
        uint32_t score{0};

        while ((x >= 0 && x < width) && (y >= 0 && y < height))
        {
            if (x != (int)startX || y != (int)startY)
            {
                ++score;
                if (map[y * width + x] >= startHeight)
                {
                    return score;
                }
            }

            x += dx;
            y += dy;
        }

        return score;
    }

    void Part2()
    {
        vector<uint32_t> map;
        uint32_t width{};

        std::string input;
        while (std::getline(std::cin, input))
        {
            width = input.length();
            for (auto i = 0; i < input.length(); ++i)
            {
                map.emplace_back(atoi(input.substr(i, 1).c_str()));
            }
        }

        const auto height = map.size() / width;

        vector<uint32_t> scores(map.size(), 0);
        for (auto x = 0UL; x < width; ++x)
        {
            for (auto y = 0UL; y < height; ++y)
            {
                auto left = Trace2(map, width, height, x, y, -1, 0);
                auto right = Trace2(map, width, height, x, y, 1, 0);
                auto up = Trace2(map, width, height, x, y, 0, -1);
                auto down = Trace2(map, width, height, x, y, 0, 1);
                scores[y * width + x] = left * right * up * down;
            }
        }

        std::cout << *std::max_element(scores.begin(), scores.end()) << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
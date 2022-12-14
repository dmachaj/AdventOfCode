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
    static int s_minX = 9999;
    static int s_maxX = 0;
    static int s_minY = 9999;
    static int s_maxY = 0;

    size_t PosToIndex(int x, int y, int width)
    {
        return (y * width) + x;
    }

    bool PosEmpty(const vector<char>& board, int x, int y, int width)
    {
        // Was useful to help find the appropriate bounds for part 2.  Breaks part 1 to throw here.
        // if (x < 0 || x >= width) throw std::exception();
        // if (y < 0 || y >= (board.size() / width)) throw std::exception();
        
        if (x < 0 || x >= width) return false;
        if (y < 0 || y >= (board.size() / width)) return false;

        const auto index = PosToIndex(x, y, width);
        if (board[index] == '.') return true;
        return false;
    }

    void MarkLine(vector<char>& board, int width, int height, const std::string& beginCoord, const std::string& endCoord)
    {
        auto beginX = std::atoi(beginCoord.substr(0, beginCoord.find(',')).c_str());
        auto beginY = std::atoi(beginCoord.substr(beginCoord.find(',') + 1).c_str());

        auto endX = std::atoi(endCoord.substr(0, endCoord.find(',')).c_str());
        auto endY = std::atoi(endCoord.substr(endCoord.find(',') + 1).c_str());

        s_minX = std::min(s_minX, beginX);
        s_minX = std::min(s_minX, endX);
        s_minY = std::min(s_minY, beginY);
        s_minY = std::min(s_minY, endY);

        s_maxX = std::max(s_maxX, beginX);
        s_maxX = std::max(s_maxX, endX);
        s_maxY = std::max(s_maxY, beginY);
        s_maxY = std::max(s_maxY, endY);

        if (beginX < 0 || endX < 0 || beginY < 0 || endY < 0) { throw std::exception(); }
        if (beginX > width || endX > width || beginY > height || endY > height) { throw std::exception(); }

        for (auto y = std::min(beginY, endY); y <= std::max(beginY, endY); ++y)
        {
            for (auto x = std::min(beginX, endX); x <= std::max(beginX, endX); ++x)
            {
                board[PosToIndex(x, y, width)] = '#';
            }
        }
    }

    void DropSandSimulation(vector<char>& board, int width, std::function<bool(std::pair<int,int>)> stopOp)
    {
        const std::pair<int, int> c_sandStart{ 500, 0 };
        auto sand = c_sandStart;
        while (true)
        {
            auto tryFall = sand;
            tryFall.second++;
            if (PosEmpty(board, tryFall.first, tryFall.second, width))
            {
                sand = tryFall;
                continue;
            }

            auto tryLeft = sand;
            tryLeft.second++;
            tryLeft.first--;
            if (PosEmpty(board, tryLeft.first, tryLeft.second, width))
            {
                sand = tryLeft;
                continue;
            }

            auto tryRight = sand;
            tryRight.second++;
            tryRight.first++;
            if (PosEmpty(board, tryRight.first, tryRight.second, width))
            {
                sand = tryRight;
                continue;
            }
            
            if (stopOp(sand))
            {
                break;
            }

            // cannot move.  stop here
            board[PosToIndex(sand.first, sand.second, width)] = 'o';

            sand = c_sandStart;
        }
    }

    void Part1()
    {
        constexpr int c_width{600};
        constexpr int c_height{200};
        vector<char> board(c_width * c_height, '.');

        // Parsing input
        std::string input;
        while (std::getline(std::cin, input))
        {
            std::string beginCoord = input.substr(0, input.find("->") - 1);
            size_t offset{1};
            while (input.find("->", offset) != std::string::npos)
            {
                auto loc = input.find("->", offset);
                size_t next = input.find("->", loc + 1);
                std::string endCoord = input.substr(loc + 3, next - loc - 4);

                MarkLine(board, c_width, c_height, beginCoord, endCoord);

                std::swap(beginCoord, endCoord);
                offset = loc + 1;
            }
        }

        // Run simulation
        DropSandSimulation(board, c_width, [](std::pair<int, int> sand)
        {
            return sand.second > s_maxY;
        });

        // Visualize the board
        for (auto y = 0; y < s_maxY + 1; ++y)
        {
            for (auto x = s_minX - 1; x < s_maxX + 1; ++x)
            {
                std::cerr << board[PosToIndex(x, y, c_width)];
            }
            std::cerr << std::endl;
        }
        std::cerr << std::endl;

        uint32_t total = std::accumulate(board.begin(), board.end(), 0, [](uint32_t total, char c) { return (c == 'o') ? (total + 1) : total; });
        std::cout << total << std::endl;
    }

    void Part2()
    {
        constexpr int c_width{700};
        constexpr int c_height{200};
        vector<char> board(c_width * c_height, '.');

        // Parsing input
        std::string input;
        while (std::getline(std::cin, input))
        {
            std::string beginCoord = input.substr(0, input.find("->") - 1);
            size_t offset{1};
            while (input.find("->", offset) != std::string::npos)
            {
                auto loc = input.find("->", offset);
                size_t next = input.find("->", loc + 1);
                std::string endCoord = input.substr(loc + 3, next - loc - 4);

                MarkLine(board, c_width, c_height, beginCoord, endCoord);

                std::swap(beginCoord, endCoord);
                offset = loc + 1;
            }
        }

        // Add a floor below the highest Y value
        const auto floor = s_maxY + 2;
        std::ostringstream baselineStart;
        baselineStart << 0 << "," << floor;
        std::ostringstream baselineEnd;
        baselineEnd << c_width << "," << floor;
        MarkLine(board, c_width, c_height, baselineStart.str(), baselineEnd.str());

        // Run simulation
        DropSandSimulation(board, c_width, [](std::pair<int, int> sand)
        {
            return sand.second == 0;
        });

        // Visualize the board
        // for (auto y = 0; y < s_maxY + 1; ++y)
        // {
        //     for (auto x = s_minX - 1; x < s_maxX + 1; ++x)
        //     {
        //         std::cerr << board[PosToIndex(x, y, c_width)];
        //     }
        //     std::cerr << std::endl;
        // }
        // std::cerr << std::endl;

        uint32_t total = std::accumulate(board.begin(), board.end(), 1, [](uint32_t total, char c) { return (c == 'o') ? (total + 1) : total; });
        std::cout << total << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
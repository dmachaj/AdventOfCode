#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "../../Includes/HelpfulInfrastructure.h"

#pragma warning (disable: 4267) // size_t <-> uint

namespace
{
    void Part1()
    {
        // True == tree is there, false == open
        std::vector<std::vector<bool>> map{};

        std::string input;
        while (std::getline(std::cin, input))
        {
            std::vector<bool> row{};
            for (const auto letter : input)
            {
                row.emplace_back(letter == '#');
            }
            map.emplace_back(std::move(row));
        }

        int treesHit{};
        const int movesNeeded = map.size();
        for (auto i = 1; i < movesNeeded; ++i) // first row is a freebie
        {
            auto& row = map[i];
            if (row[(i * 3) % row.size()])
            {
                treesHit++;
            }
        }

        std::cout << treesHit << std::endl;
    }

    void Part2()
    {
        // True == tree is there, false == open
        std::vector<std::vector<bool>> map{};

        std::string input;
        while (std::getline(std::cin, input))
        {
            std::vector<bool> row{};
            for (const auto letter : input)
            {
                row.emplace_back(letter == '#');
            }
            map.emplace_back(std::move(row));
        }
        
        uint64_t treesHit1x1{};
        for (auto i = 1; i < map.size(); ++i) // first row is a freebie
        {
            auto& row = map[i];
            if (row[(i) % row.size()])
            {
                treesHit1x1++;
            }
        }

        uint64_t treesHit3x1{};
        for (auto i = 1; i < map.size(); ++i) // first row is a freebie
        {
            auto& row = map[i];
            if (row[(i * 3) % row.size()])
            {
                treesHit3x1++;
            }
        }

        uint64_t treesHit5x1{};
        for (auto i = 1; i < map.size(); ++i) // first row is a freebie
        {
            auto& row = map[i];
            if (row[(i * 5) % row.size()])
            {
                treesHit5x1++;
            }
        }

        uint64_t treesHit7x1{};
        for (auto i = 1; i < map.size(); ++i) // first row is a freebie
        {
            auto& row = map[i];
            if (row[(i * 7) % row.size()])
            {
                treesHit7x1++;
            }
        }

        uint64_t treesHit1x2{};
        for (auto i = 2; i < map.size(); i += 2) // first row is a freebie
        {
            auto& row = map[i];
            if (row[(i / 2) % row.size()])
            {
                treesHit1x2++;
            }
        }

        std::cerr << "1x1: " << treesHit1x1 << std::endl;
        std::cerr << "3x1: " << treesHit3x1 << std::endl;
        std::cerr << "5x1: " << treesHit5x1 << std::endl;
        std::cerr << "7x1: " << treesHit7x1 << std::endl;
        std::cerr << "1x2: " << treesHit1x2 << std::endl;
        std::cout << treesHit1x1 * treesHit3x1 * treesHit5x1 * treesHit7x1 * treesHit1x2<< std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
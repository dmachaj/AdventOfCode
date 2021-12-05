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
    void Part1()
    {
        constexpr uint32_t c_size = 1000;
        std::array<std::array<uint32_t, c_size>, c_size> board;
        
        for (auto& outer : board)
        {
            for (auto& inner : outer)
            {
                inner = 0;
            }
        }

        std::string input;
        while (std::getline(std::cin, input))
        {
            int x1 = std::atoi(input.substr(0, input.find(",")).c_str());
            int y1 = std::atoi(input.substr(input.find(",") + 1, input.find(" ")).c_str());

            std::string_view second(input.c_str() + input.find('>') + 2);
            int x2 = std::atoi(second.substr(0, second.find(",")).data());
            int y2 = std::atoi(second.substr(second.find(",") + 1).data());

            if (x1 == x2) // vertical line
            {
                for (auto i = std::min(y1, y2); i <= std::max(y1, y2); ++i)
                {
                    board[i][x1]++;
                }
            }
            else if (y1 == y2)
            {
                for (auto i = std::min(x1, x2); i <= std::max(x1, x2); ++i)
                {
                    board[y1][i]++;
                }
            }
            else
            {
                // ignore for part 1
            }
        }

        uint32_t result{};
        for (const auto& outer : board)
        {
            for (const auto& inner : outer)
            {
                if (inner >= 2)
                {
                    ++result;
                }
            }
        }

        std::cout << result << std::endl;
    }

    void Part2()
    {
        constexpr uint32_t c_size = 1000;
        std::array<std::array<uint32_t, c_size>, c_size> board;
        
        for (auto& outer : board)
        {
            for (auto& inner : outer)
            {
                inner = 0;
            }
        }

        std::string input;
        while (std::getline(std::cin, input))
        {
            int x1 = std::atoi(input.substr(0, input.find(",")).c_str());
            int y1 = std::atoi(input.substr(input.find(",") + 1, input.find(" ")).c_str());

            std::string_view second(input.c_str() + input.find('>') + 2);
            int x2 = std::atoi(second.substr(0, second.find(",")).data());
            int y2 = std::atoi(second.substr(second.find(",") + 1).data());

            if (x1 == x2) // vertical line
            {
                for (auto i = std::min(y1, y2); i <= std::max(y1, y2); ++i)
                {
                    board[i][x1]++;
                }
            }
            else if (y1 == y2)
            {
                for (auto i = std::min(x1, x2); i <= std::max(x1, x2); ++i)
                {
                    board[y1][i]++;
                }
            }
            else
            {
                const bool xIncreases = x1 < x2;
                const bool yIncreases = y1 < y2;

                const auto steps = (uint32_t)std::abs(x1 - x2);
                auto x = x1;
                auto y = y1;
                for (auto i = 0UL; i <= steps; ++i)
                {
                    board[y][x]++;
                    if (xIncreases)
                    {
                        ++x;
                    }
                    else
                    {
                        --x;
                    }

                    if (yIncreases)
                    {
                        ++y;
                    }
                    else
                    {
                        --y;
                    }
                }
            }
        }

        uint32_t result{};
        for (const auto& outer : board)
        {
            for (const auto& inner : outer)
            {
                if (inner >= 2)
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
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
    void RaiseNeighbors(std::vector<uint32_t>& board, uint32_t pos, uint32_t width)
    {
        if ((pos - width < board.size()) && ((pos % width) != 0)) ++board[pos - width - 1];
        if (pos - width < board.size()) ++board[pos - width];
        if ((pos - width < board.size()) && ((pos + 1) % width != 0)) ++board[pos - width + 1];

        if (pos % width != 0) ++board[pos - 1];
        if ((pos + 1) % width != 0) ++board[pos + 1];

        if ((pos + width < board.size()) && (pos % width != 0)) ++board[pos + width - 1];
        if (pos + width < board.size()) ++board[pos + width];
        if ((pos + width < board.size()) && ((pos + 1) % width != 0)) ++board[pos + width + 1];
    }

    void Part1()
    {
        std::vector<uint32_t> board;
        constexpr uint32_t c_width{10};

        std::string input;
        while (std::getline(std::cin, input))
        {
            for (const auto c : input)
            {
                board.emplace_back(c - '0');
            }
        }

        uint32_t numRaises{};
        for (auto i = 0UL; i < 100UL; ++i)
        {
            std::vector<bool> raised(board.size(), false);

            for (auto& pos : board)
            {
                ++pos;
            }

            while (true)
            {
                bool needRaise{};
                for (auto x = 0UL; x < board.size(); ++x)
                {
                    if (board[x] > 9 && !raised[x])
                    {
                        needRaise = true;
                        break;
                    }
                }

                if (!needRaise) break;

                for (auto x = 0UL; x < board.size(); ++x)
                {
                    if (!raised[x] && board[x] >= 10)
                    {
                        raised[x] = true;
                        ++numRaises;
                        RaiseNeighbors(board, x, c_width);
                    }
                }
            }

            for (auto& pos : board)
            {
                if (pos > 9) pos = 0;
            }
        }

        std::cout << numRaises << std::endl;
    }

    void Part2()
    {
        std::vector<uint32_t> board;
        constexpr uint32_t c_width{10};

        std::string input;
        while (std::getline(std::cin, input))
        {
            for (const auto c : input)
            {
                board.emplace_back(c - '0');
            }
        }

        uint32_t allFlashCounter{1};
        for (;; ++allFlashCounter)
        {
            std::vector<bool> raised(board.size(), false);

            for (auto& pos : board)
            {
                ++pos;
            }

            while (true)
            {
                bool needRaise{};
                for (auto x = 0UL; x < board.size(); ++x)
                {
                    if (board[x] > 9 && !raised[x])
                    {
                        needRaise = true;
                        break;
                    }
                }

                if (!needRaise) break;

                for (auto x = 0UL; x < board.size(); ++x)
                {
                    if (!raised[x] && board[x] >= 10)
                    {
                        raised[x] = true;
                        RaiseNeighbors(board, x, c_width);
                    }
                }
            }

            for (auto& pos : board)
            {
                if (pos > 9) pos = 0;
            }

            if (std::all_of(board.begin(), board.end(), [](auto pos) { return pos == 0; }))
            {
                break;
            }
        }

        std::cout << allFlashCounter << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
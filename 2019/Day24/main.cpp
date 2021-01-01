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
    void PrintBoard(const std::vector<bool>& board)
    {
        for (auto i = 0UL; i < board.size(); ++i)
        {
            if (board[i])
            {
                std::cerr << "#";
            }
            else
            {
                std::cerr << ".";
            }

            if (((i + 1) % 5) == 0)
            {
                std::cerr << std::endl;
            }
        }
        std::cerr << std::endl;
    }

    bool CheckSquare(const std::vector<bool>& board, int32_t x, int32_t y)
    {
        if ((x < 0) || (x >= 5))
        {
            return false;
        }

        if ((y < 0) || (y >= 5))
        {
            return false;
        }

        auto index = (y * 5) + x;
        return board[index];
    }

    void Part1()
    {
        std::vector<bool> board;
        {
            std::string input;
            while (std::getline(std::cin, input))
            {
                for(auto letter : input)
                {
                    board.emplace_back(letter == '#');
                }
            }
            if (board.size() != 25) { throw; }
        }
        // PrintBoard(board);

        std::set<std::vector<bool>> allBoards { board };
        while (true)
        {
            auto boardCopy = board;
            for (auto y = 0; y < 5; ++y)
            {
                for (auto x = 0; x < 5; ++x)
                {
                    auto up = CheckSquare(boardCopy, x, y - 1);
                    auto down = CheckSquare(boardCopy, x, y + 1);
                    auto left = CheckSquare(boardCopy, x - 1, y);
                    auto right = CheckSquare(boardCopy, x + 1, y);

                    uint32_t total{};
                    if (up) ++total;
                    if (down) ++total;
                    if (left) ++total;
                    if (right) ++total;

                    const auto index = (y * 5) + x;
                    if (boardCopy[index] && (total != 1))
                    {
                        board[index] = false;
                    }

                    if (!boardCopy[index] && ((total == 1) || (total == 2)))
                    {
                        board[index] = true;
                    }
                }
            }

            // PrintBoard(board);
            if (allBoards.find(board) != allBoards.end())
            {
                break;
            }
            allBoards.emplace(board);
        }

        uint64_t result{};
        uint64_t multiplier{1ULL};
        for (auto entry : board)
        {
            if (entry)
            {
                result += multiplier;
            }
            multiplier *= 2;
        }

        std::cout << result << std::endl;
    }

    void Part2()
    {
        uint32_t iterationCount{};
        std::vector<bool> board;
        {
            std::string input;
            std::getline(std::cin, input);
            iterationCount = (uint32_t)std::atoi(input.c_str());

            while (std::getline(std::cin, input))
            {
                for(auto letter : input)
                {
                    board.emplace_back(letter == '#');
                }
            }
            if (board.size() != 25) { throw; }
        }
        // PrintBoard(board);

        std::set<std::vector<bool>> allBoards { board };
        while (true)
        {
            auto boardCopy = board;
            for (auto y = 0; y < 5; ++y)
            {
                for (auto x = 0; x < 5; ++x)
                {
                    auto up = CheckSquare(boardCopy, x, y - 1);
                    auto down = CheckSquare(boardCopy, x, y + 1);
                    auto left = CheckSquare(boardCopy, x - 1, y);
                    auto right = CheckSquare(boardCopy, x + 1, y);

                    uint32_t total{};
                    if (up) ++total;
                    if (down) ++total;
                    if (left) ++total;
                    if (right) ++total;

                    const auto index = (y * 5) + x;
                    if (boardCopy[index] && (total != 1))
                    {
                        board[index] = false;
                    }

                    if (!boardCopy[index] && ((total == 1) || (total == 2)))
                    {
                        board[index] = true;
                    }
                }
            }

            // PrintBoard(board);
            if (allBoards.find(board) != allBoards.end())
            {
                break;
            }
            allBoards.emplace(board);
        }

        uint64_t result{};
        uint64_t multiplier{1ULL};
        for (auto entry : board)
        {
            if (entry)
            {
                result += multiplier;
            }
            multiplier *= 2;
        }

        std::cout << result << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
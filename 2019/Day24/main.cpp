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

    using AllBoards = std::map<int32_t, std::vector<bool>>;
    enum class Direction
    {
        Up,
        Down,
        Left,
        Right
    };
    uint32_t CheckSquareV2(const AllBoards& allBoards, int32_t x, int32_t y, int32_t recursionDepth, Direction direction)
    {
        if (x == 2 && y == 2) // Recursing down a level
        {
            // Don't recurse deeper than we have to
            if (allBoards.find(recursionDepth + 1) == allBoards.end()) return 0UL;

            const auto& board = allBoards.at(recursionDepth + 1);
            uint32_t result{};
            switch (direction)
            {
                case Direction::Up:
                    for (auto xIter = 0UL; xIter < 5UL; ++xIter)
                    {
                        auto index = (4 * 5) + xIter;
                        if (board[index]) ++result;
                    }
                    break;

                case Direction::Down:
                    for (auto xIter = 0UL; xIter < 5UL; ++xIter)
                    {
                        auto index = (0 * 5) + xIter;
                        if (board[index]) ++result;
                    }
                    break;

                case Direction::Left:
                    for (auto yIter = 0UL; yIter < 5UL; ++yIter)
                    {
                        auto index = (yIter * 5) + 4;
                        if (board[index]) ++result;
                    }
                    break;

                case Direction::Right:
                    for (auto yIter = 0UL; yIter < 5UL; ++yIter)
                    {
                        auto index = (yIter * 5) + 0;
                        if (board[index]) ++result;
                    }
                    break;
            }
            return result;
        }
        else // current level or recursing up one
        {
            if (x < 0)
            {
                x = 1;
                y = 2;
                --recursionDepth;
            }
            else if (y < 0)
            {
                x = 2;
                y = 1;
                --recursionDepth;
            }
            else if (x >= 5)
            {
                x = 3;
                y = 2;
                --recursionDepth;
            }
            else if (y >= 5)
            {
                x = 2;
                y = 3;
                --recursionDepth;
            }

            if (allBoards.find(recursionDepth) == allBoards.end()) return 0UL;
            auto index = (y * 5) + x;
            return allBoards.at(recursionDepth)[index] ? 1UL : 0UL;
        }
    }

    void Part2()
    {
        uint32_t iterationCount{};
        AllBoards allBoards;
        {
            std::vector<bool> board;
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
            PrintBoard(board);
            allBoards[0] = std::move(board);
        }

        // every iteration we will grow the map by one board :|
        for (auto i = 1UL; i <= iterationCount; ++i)
        {
            std::vector<bool> blankBoard(25, false);
            allBoards[i] = blankBoard;
            allBoards[-1 * (int32_t)i] = blankBoard;
        }

        for (auto i = 0UL; i < iterationCount; ++i)
        {
            const auto allBoardsCopy = allBoards;
            for (auto z = -((int32_t)iterationCount); z < (int32_t)iterationCount; ++z)
            {
                for (auto y = 0; y < 5; ++y)
                {
                    for (auto x = 0; x < 5; ++x)
                    {
                        if (x == 2 && y == 2) continue; // recursion square

                        auto up = CheckSquareV2(allBoardsCopy, x, y - 1, z, Direction::Up);
                        auto down = CheckSquareV2(allBoardsCopy, x, y + 1, z, Direction::Down);
                        auto left = CheckSquareV2(allBoardsCopy, x - 1, y, z, Direction::Left);
                        auto right = CheckSquareV2(allBoardsCopy, x + 1, y, z, Direction::Right);
                        const uint32_t total{ up + down + left + right};

                        auto& currentBoard = allBoards[z];
                        const auto index = (y * 5) + x;
                        if (allBoardsCopy.at(z)[index] && (total != 1))
                        {
                            currentBoard[index] = false;
                        }

                        if (!allBoardsCopy.at(z)[index] && ((total == 1) || (total == 2)))
                        {
                            currentBoard[index] = true;
                        }
                    }
                }
            }

            // PrintBoard(board);
        }

        uint64_t result{};
        for (const auto& board : allBoards)
        {
            for (auto square : board.second)
            {
                if (square)
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
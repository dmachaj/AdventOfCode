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

std::vector<bool> ParseInput(const std::string& input)
{
    std::vector<bool> moves;
    for (char c : input)
    {
        if (c == '<')
        {
            moves.emplace_back(true);
        }
        else
        {
            moves.emplace_back(false);
        }
    }
    return moves;
}

using Piece = std::vector<int32_t>;

constexpr int32_t c_pieceCount{ 2022 };
constexpr int32_t c_boardWidth{7};

Piece SpawnBar(int32_t x, int32_t y)
{
    Piece result{
        y * c_boardWidth + x,
        y * c_boardWidth + x + 1,
        y * c_boardWidth + x + 2,
        y * c_boardWidth + x + 3,
    };
    return result;
}

Piece SpawnPlus(int32_t x, int32_t y)
{
    Piece result{
        (y + 2) * c_boardWidth + x + 1,
        (y + 1) * c_boardWidth + x,
        (y + 1) * c_boardWidth + x + 1,
        (y + 1) * c_boardWidth + x + 2,
        y * c_boardWidth + x + 1,
    };
    return result;
}

Piece SpawnL(int32_t x, int32_t y)
{
    Piece result{
        (y + 2) * c_boardWidth + x + 2,
        (y + 1) * c_boardWidth + x + 2,
        y * c_boardWidth + x + 2,
        y * c_boardWidth + x + 1,
        y * c_boardWidth + x,
    };
    return result;
}

Piece SpawnI(int32_t x, int32_t y)
{
    Piece result{
        (y + 3) * c_boardWidth + x,
        (y + 2) * c_boardWidth + x,
        (y + 1) * c_boardWidth + x,
        y * c_boardWidth + x,
    };
    return result;
}

Piece SpawnBox(int32_t x, int32_t y)
{
    Piece result{
        (y + 1) * c_boardWidth + x,
        (y + 1) * c_boardWidth + x + 1,
        y * c_boardWidth + x,
        y * c_boardWidth + x + 1,
    };
    return result;
}

void PrintBottom(const std::vector<char>& board, const Piece& curr, int32_t rowCount)
{
    auto boardCopy = board;
    for (auto box : curr)
    {
        boardCopy[box] = '@';
    }

    for (auto y = rowCount; y >= 0; --y)
    {
        std::cerr << "|";
        for (auto x = 0; x < c_boardWidth; ++x)
        {
            std::cerr << boardCopy[y * c_boardWidth + x];
        }
        std::cerr << "|"<< std::endl;
    }
    std::cerr << "+-------+" << std::endl;
}

void TryMoveLeft(const std::vector<char>& board, Piece& piece)
{
    Piece copy = piece;
    for (auto& box : copy)
    {
        const auto before = box / c_boardWidth;
        --box;
        const auto after = box / c_boardWidth;

        if (before != after)
        {
            return; // hit left wall
        }
        else if (board[box] == '#')
        {
            return; // hit another piece
        }
    }

    std::swap(copy, piece);
}

void TryMoveRight(const std::vector<char>& board, Piece& piece)
{
    Piece copy = piece;
    for (auto& box : copy)
    {
        const auto before = box / c_boardWidth;
        ++box;
        const auto after = box / c_boardWidth;

        if (before != after)
        {
            return; // hit left wall
        }
        else if (board[box] == '#')
        {
            return; // hit another piece
        }
    }

    std::swap(copy, piece);
}

bool TryMoveDown(const std::vector<char>& board, Piece& piece)
{
    Piece copy = piece;
    for (auto& box : copy)
    {
        box -= c_boardWidth;
    }

    for (const auto& box : copy)
    {
        if (box < 0)
        {
            return true;
        }
        else if (board[box] == '#')
        {
            return true;
        }
    }

    std::swap(copy, piece);
    return false;
}

int32_t RunSimulation(const std::vector<bool>& moves, int32_t cycleCount)
{
    std::vector<char> board(c_boardWidth * 5, '.'); // pre-allocate 5 rows (arbitrary)

    int32_t highestPoint{};
    int32_t moveNumber{};

    std::vector<std::function<Piece(int32_t, int32_t)>> spawners =
        { SpawnBar, SpawnPlus, SpawnL, SpawnI, SpawnBox };

    for (auto i = 0; i < cycleCount; ++i)
    {
        const auto newHighest = ((highestPoint + 7) * c_boardWidth);
        while (newHighest > board.size())
        {
            board.emplace_back('.');
            board.emplace_back('.');
            board.emplace_back('.');
            board.emplace_back('.');
            board.emplace_back('.');
            board.emplace_back('.');
            board.emplace_back('.');
        }

        // Spawn piece
        auto curr = spawners[i % spawners.size()](2, highestPoint + 3);

        // Move piece until it lands
        while (true)
        {
            auto currentMove = moves[moveNumber++ % moves.size()];
            // PrintBottom(board, curr, 10);
            if (currentMove)
            {
                TryMoveLeft(board, curr);
            }
            else
            {
                TryMoveRight(board, curr);
            }

            // PrintBottom(board, curr, 10);
            bool landed = TryMoveDown(board, curr);
            if (landed) break;
        }

        // Commit piece
        for (auto box : curr)
        {
            board[box] = '#';
            const auto y = (box / c_boardWidth) + 1;
            highestPoint = std::max(highestPoint, y);
        }
        // PrintBottom(board, curr, 10);
    }
    return highestPoint;
}

void Part1()
{
    std::string input;
    std::getline(std::cin, input);
    const auto moves = ParseInput(input); // true == left
    const auto highestPoint = RunSimulation(moves, 2022);
    std::cout << highestPoint << std::endl;
}

void Part2()
{
    std::string input;
    std::getline(std::cin, input);
    const auto moves = ParseInput(input); // true == left

    int32_t cycleCount{};
    for (auto i = moves.size(); i < std::numeric_limits<int32_t>().max(); ++i)
    {
        if (i % 100 == 0) { std::cerr << "Iteration count: " << i << std::endl; }

        const auto highestPointBase = RunSimulation(moves, i);
        const auto highestPointDoubled = RunSimulation(moves, i * 2);

        if (highestPointBase * 2 == highestPointDoubled)
        {
            cycleCount = i;
            break;
        }
    }

    // const auto highestPoint = RunSimulation(moves, 2022);

    std::cout << cycleCount << std::endl;
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
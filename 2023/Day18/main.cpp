#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
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

void RunCommand(std::vector<bool>& board, int size, std::string const& command, std::pair<int, int>& currentPosition)
{
    const auto direction = command[0];
    const auto amount = std::atoi(command.substr(2, command.find(' ', 2)).c_str());

    for (auto i = 0; i < amount; ++i)
    {
        switch (direction)
        {
        case 'U':
            currentPosition.second--;
            break;
        case 'D':
            currentPosition.second++;
            break;
        case 'L':
            currentPosition.first--;
            break;
        case 'R':
            currentPosition.first++;
            break;
        default: throw std::exception();
        }

        if (currentPosition.first < 0 || currentPosition.second < 0)
        {
            throw std::exception();
        }
        else if (currentPosition.first >= size || currentPosition.second >= size)
        {
            throw std::exception();
        }

        board[(currentPosition.second * size) + currentPosition.first] = true;
    }
}

void DumpBoard(std::vector<bool> const& board, int size)
{
    return;

    // for (auto y = 0; y < size; ++y)
    // {
    //     for (auto x = 0; x < size; ++x)
    //     {
    //         std::cerr << (board[(y * size) + x] ? '#' : '.');
    //     }
    //     std::cerr << std::endl;
    // }
    // std::cerr << std::endl;
}

uint64_t CountEmpty(std::vector<bool>& board, int size)
{
    struct Flood
    {
        int X{};
        int Y{};
        
        size_t ToIndex(int size) const { return Y * size + X; }

        bool operator==(Flood const& other) const
        {
            return other.X == X && other.Y == Y;
        }

        bool IsValid(int size) const
        {
            return (X >= 0 && X < size) && (Y >= 0 && Y < size);
        }
    };

    struct FloodHash
    {
        size_t operator()(const Flood& pt) const noexcept
        {
            auto h1 = std::hash<int32_t>{}(pt.X);
            auto h2 = std::hash<int32_t>{}(pt.Y);
            return h1 ^ (h2 << 1);
        }
    };

    std::vector<bool> emptySpaces(size * size, false);

    std::unordered_set<Flood, FloodHash> pendingMoves;
    pendingMoves.emplace(Flood{});
    std::unordered_set<Flood, FloodHash> completedMoves;
    while (!pendingMoves.empty())
    {
        auto move = *pendingMoves.begin();
        pendingMoves.erase(pendingMoves.begin());

        const auto isFilled = board[move.ToIndex(size)];
        completedMoves.emplace(move);

        if (isFilled) continue;

        emptySpaces[move.ToIndex(size)] = true;

        auto up = move;
        up.Y--;
        if (up.IsValid(size) &&
            completedMoves.find(up) == completedMoves.end() &&
            pendingMoves.find(up) == pendingMoves.end())
        {
            pendingMoves.emplace(up);
        }

        auto down = move;
        down.Y++;
        if (down.IsValid(size) &&
            completedMoves.find(down) == completedMoves.end() &&
            pendingMoves.find(down) == pendingMoves.end())
        {
            pendingMoves.emplace(down);
        }
        
        auto left = move;
        left.X--;
        if (left.IsValid(size) &&
            completedMoves.find(left) == completedMoves.end() &&
            pendingMoves.find(left) == pendingMoves.end())
        {
            pendingMoves.emplace(left);
        }
        
        auto right = move;
        right.X++;
        if (right.IsValid(size) &&
            completedMoves.find(right) == completedMoves.end() &&
            pendingMoves.find(right) == pendingMoves.end())
        {
            pendingMoves.emplace(right);
        }
    }

    DumpBoard(emptySpaces, size);

    return std::accumulate(emptySpaces.begin(), emptySpaces.end(), 0ULL);
}

void Part1()
{
    int boardSize;
    std::cin >> boardSize;
    std::cin.get(); // eat newline

    std::vector<bool> board(boardSize * boardSize, false);

    std::pair<int, int> currentPosition{ boardSize / 3, boardSize / 3};
    std::string input;
    while (std::getline(std::cin, input))
    {
        RunCommand(board, boardSize, input, currentPosition);
    }

    DumpBoard(board, boardSize);
    auto result = (boardSize * boardSize) - CountEmpty(board, boardSize);

    std::cout << result << std::endl;
}

void Part2()
{
    std::cout << 0 << std::endl;
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
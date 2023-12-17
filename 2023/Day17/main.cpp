#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <iomanip>
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

constexpr int c_empty{99999};

enum class Dir
{
    Up, Down, Left, Right
};

bool InBounds(int width, int height, int x, int y)
{
    if (x < 0 || x >= width) return false;
    if (y < 0 || y >= height) return false;
    return true;
}

template <typename T>
uint32_t Lookup(T const& board, int width, int height, int x, int y)
{
    if (!InBounds(width, height, x, y)) return c_empty;
    const auto index = y * width + x;
    return board[index];
}

void ComputePathRecursive(std::vector<int> const& board, int width, int height, std::vector<int>& costs,
    int x, int y, int numInRow, Dir dir)
{
    const auto currentCost = Lookup(costs, width, height, x, y);

    // Look up
    if ((dir != Dir::Down) && InBounds(width, height, x, y - 1))
    {
        if ((dir == Dir::Up) && (numInRow + 1) >= 3)
        {}
        else
        {
            const auto val = Lookup(board, width, height, x, y - 1);
            const auto newCost = currentCost + val;
            if (newCost < Lookup(costs, width, height, x, y - 1))
            {
                const auto newInRow = (dir == Dir::Up) ? (numInRow + 1) : 0;
                costs[(y - 1) * width + x] = newCost;
                ComputePathRecursive(board, width, height, costs, x, y - 1, newInRow, Dir::Up);
            }
        }
    }

    // Look down
    if ((dir != Dir::Up) && InBounds(width, height, x, y + 1))
    {
        if ((dir == Dir::Down) && (numInRow + 1) >= 3)
        {}
        else
        {
            const auto val = Lookup(board, width, height, x, y + 1);
            const auto newCost = currentCost + val;
            if (newCost < Lookup(costs, width, height, x, y + 1))
            {
                const auto newInRow = (dir == Dir::Down) ? (numInRow + 1) : 0;
                costs[(y + 1) * width + x] = newCost;
                ComputePathRecursive(board, width, height, costs, x, y + 1, newInRow, Dir::Down);
            }
        }
    }

    // Look right
    if ((dir != Dir::Left) && InBounds(width, height, x + 1, y))
    {
        if ((dir == Dir::Right) && (numInRow + 1) >= 3)
        {}
        else
        {
            const auto val = Lookup(board, width, height, x + 1, y);
            const auto newCost = currentCost + val;
            if (newCost < Lookup(costs, width, height, x + 1, y))
            {
                const auto newInRow = (dir == Dir::Right) ? (numInRow + 1) : 0;
                costs[(y) * width + x + 1] = newCost;
                ComputePathRecursive(board, width, height, costs, x + 1, y, newInRow, Dir::Right);
            }
        }
    }

    // Look left
    if ((dir != Dir::Right) && InBounds(width, height, x - 1, y))
    {
        if ((dir == Dir::Left) && (numInRow + 1) >= 3)
        {}
        else
        {
            const auto val = Lookup(board, width, height, x - 1, y);
            const auto newCost = currentCost + val;
            if (newCost < Lookup(costs, width, height, x - 1, y))
            {
                const auto newInRow = (dir == Dir::Left) ? (numInRow + 1) : 0;
                costs[(y) * width + x - 1] = newCost;
                ComputePathRecursive(board, width, height, costs, x - 1, y, newInRow, Dir::Left);
            }
        }
    }
}

void Part1()
{
    std::vector<int> board;

    int width{};
    int height{};
    std::string input;
    while (std::getline(std::cin, input))
    {
        width = input.size();
        ++height;

        for (char c : input)
        {
            board.emplace_back(c - '0');
        }
    }

    std::vector<int> costs((width * height), c_empty);
    costs[0] = 0;
    ComputePathRecursive(board, width, height, costs, 0, 0, 0, Dir::Right);
    ComputePathRecursive(board, width, height, costs, 0, 0, 0, Dir::Right);

    for (auto y = 0; y < height; ++y)
    {
        for (auto x = 0; x < width; ++x)
        {
            std::cerr << std::setw(4) << costs[y * width + x];
        }
        std::cerr << std::endl;
    }

    std::cout << costs[costs.size() - 1] << std::endl;
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
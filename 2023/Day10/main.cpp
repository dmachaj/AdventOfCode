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

char SampleAt(std::string const& board, int width, int height, int x, int y)
{
    constexpr char c_empty = '.';
    if (x < 0 || x >= width) return c_empty;
    if (y < 0 || y >= height) return c_empty;
    return board[(y * width) + x];
}

char FigureOutStart(std::string const& board, int width, int height, int x, int y)
{
    const auto up = SampleAt(board, width, height, x, y - 1);
    const auto down = SampleAt(board, width, height, x, y + 1);
    const auto left = SampleAt(board, width, height, x - 1, y);
    const auto right = SampleAt(board, width, height, x + 1, y);

    const bool upConnects = (up == '|' || up == '7' || up == 'F');
    const bool leftConnects = (left == '-' || left == 'L' || left == 'F');
    const bool downConnects = (down == '|' || down == 'L' || down == 'J');
    const bool rightConnects = (right == '-' || right == 'J' || right == '7');

    if (upConnects && leftConnects) return 'J';
    else if (upConnects && rightConnects) return 'L';
    else if (upConnects && downConnects) return '|';
    else if (leftConnects && rightConnects) return '-';
    else if (leftConnects && downConnects) return '7';
    else if (rightConnects && downConnects) return 'F';
    else throw std::exception();
}

std::pair<std::pair<int, int>, std::pair<int, int>> MovesFromLocation(std::string const& board,
    int width, int height, int x, int y, char c)
{
    switch (c)
    {
    case '|':
        return std::make_pair(std::make_pair(x, y - 1), std::make_pair(x, y + 1));
    case '-':
        return std::make_pair(std::make_pair(x - 1, y), std::make_pair(x + 1, y));
    case 'L':
        return std::make_pair(std::make_pair(x, y - 1), std::make_pair(x + 1, y));
    case 'J':
        return std::make_pair(std::make_pair(x, y - 1), std::make_pair(x - 1, y));
    case '7':
        return std::make_pair(std::make_pair(x, y + 1), std::make_pair(x - 1, y));
    case 'F':
        return std::make_pair(std::make_pair(x, y + 1), std::make_pair(x + 1, y));
    }
    throw std::exception();
}

void Part1()
{
    std::string board;
    int width{};
    int height{};

    std::string input;
    while (std::getline(std::cin, input))
    {
        width = input.size();
        ++height;
        board.append(std::move(input));
    }

    size_t startIndex{};
    for (auto i = 0; i < board.size(); ++i)
    {
        if (board[i] == 'S')
        {
            startIndex = i;
            break;
        }
    }
    
    std::pair<int, int> start =
    {
        startIndex % width,
        startIndex / width
    };

    constexpr int c_unvisited = -1;
    const auto startLetter = FigureOutStart(board, width, height, start.first, start.second);
    std::vector<int> costs(board.size(), c_unvisited);
    std::queue<std::tuple<int, int, int>> moves; // x, y, cost
    moves.emplace(std::tuple<int, int, int>{start.first, start.second, 0});

    while (!moves.empty())
    {
        const auto move = moves.front();
        moves.pop();

        int x, y, cost;
        std::tie(x, y, cost) = move;

        const auto index = y * width + x;
        if (costs[index] != c_unvisited)
        {
            continue;
        }

        costs[index] = cost;

        auto currentLetter = SampleAt(board, width, height, x, y);
        if (currentLetter == 'S') currentLetter = startLetter;

        auto [first, second] = MovesFromLocation(board, width, height, x, y, currentLetter);

        std::tuple<int, int, int> firstMove { first.first, first.second, cost + 1};
        std::tuple<int, int, int> secondMove { second.first, second.second, cost + 1};

        moves.emplace(std::move(firstMove));
        moves.emplace(std::move(secondMove));
    }

     std::cout << *std::max_element(costs.begin(), costs.end()) << std::endl;
}

void Part2()
{
    std::string board;
    int width{};
    int height{};

    std::string input;
    while (std::getline(std::cin, input))
    {
        width = input.size();
        ++height;
        board.append(std::move(input));
    }

    size_t startIndex{};
    for (auto i = 0; i < board.size(); ++i)
    {
        if (board[i] == 'S')
        {
            startIndex = i;
            break;
        }
    }
    
    std::pair<int, int> start =
    {
        startIndex % width,
        startIndex / width
    };

    constexpr char c_unvisited = '.';
    const auto startLetter = FigureOutStart(board, width, height, start.first, start.second);
    std::vector<char> costs(board.size(), c_unvisited);
    std::queue<std::pair<int, int>> moves; // x, y
    moves.emplace(std::pair<int, int>{start.first, start.second});

    while (!moves.empty())
    {
        const auto move = moves.front();
        moves.pop();

        int x, y;
        std::tie(x, y) = move;

        const auto index = y * width + x;
        if (costs[index] != c_unvisited)
        {
            continue;
        }

        costs[index] = 'X';

        auto currentLetter = SampleAt(board, width, height, x, y);
        if (currentLetter == 'S') currentLetter = startLetter;

        auto [first, second] = MovesFromLocation(board, width, height, x, y, currentLetter);

        std::pair<int, int> firstMove { first.first, first.second};
        std::pair<int, int> secondMove { second.first, second.second};

        moves.emplace(std::move(firstMove));
        moves.emplace(std::move(secondMove));
    }

    for (auto i = 0; i < costs.size(); ++i)
    {
        std::cerr << costs[i];
        if ((i + 1) % width == 0) std::cerr << "\n";
    }

     std::cout << *std::max_element(costs.begin(), costs.end()) << std::endl;
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
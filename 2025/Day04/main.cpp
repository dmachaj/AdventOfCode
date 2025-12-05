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

void Part1()
{
    int width = 0;
    std::vector<bool> board;

    std::string input;
    while (std::getline(std::cin, input))
    {
        width = input.length();
        for (auto c : input)
        {
            if (c == '@') board.push_back(true);
            else board.push_back(false);
        }
    }

    // for (auto i = 0; i < board.size(); ++i)
    // {
    //     std::cerr << (board[i] ? '@' : '.');
    //     if ((i+1) % width == 0) std::cerr << std::endl;
    // }
    // std::cerr << std::endl;

    std::vector<bool> boardSolved;
    for (auto i = 0; i < board.size(); ++i)
    {
        if (!board[i])
        {
            boardSolved.push_back(false);
            continue;
        }

        const auto leftEdge = (i % width) == 0;
        const auto rightEdge = (i + 1) % width == 0;
        const auto topEdge = i < width;
        const auto bottomEdge = (board.size() - width) < i;

        const auto left = leftEdge || !board[i - 1];
        const auto right = rightEdge || !board[i + 1];
        const auto top = topEdge || !board[i - width];
        const auto bottom = bottomEdge || !board[i + width];

        const auto topLeft = leftEdge || topEdge || !board[i - width - 1];
        const auto topRight = rightEdge || topEdge || !board[i - width + 1];
        const auto bottomLeft = leftEdge || bottomEdge || !board[i + width - 1];
        const auto bottomRight = rightEdge || bottomEdge || !board[i + width + 1];

        int count{};
        if (left) ++count;
        if (right) ++count;
        if (top) ++count;
        if (bottom) ++count;

        if (topLeft) ++count;
        if (topRight) ++count;
        if (bottomLeft) ++count;
        if (bottomRight) ++count;

        // Less than 4 directions should be blocked.  aka at least 5 should be free
        if (count >= 5) boardSolved.push_back(true);
        else boardSolved.push_back(false);
    }

    // for (auto i = 0; i < boardSolved.size(); ++i)
    // {
    //     std::cerr << (boardSolved[i] ? 'x' : (board[i] ? '@' : '.'));
    //     if ((i+1) % width == 0) std::cerr << std::endl;
    // }
    // std::cerr << std::endl;

    int result = std::accumulate(boardSolved.begin(), boardSolved.end(), 0, [](int sum, bool val) { return val ? (sum + 1) : sum; });
    std::cout << result << std::endl;
}

void Part2()
{
    int width = 0;
    std::vector<bool> board;

    std::string input;
    while (std::getline(std::cin, input))
    {
        width = input.length();
        for (auto c : input)
        {
            if (c == '@') board.push_back(true);
            else board.push_back(false);
        }
    }

    std::vector<bool> boardSolved = board;
    bool dirty = true;
    while (dirty)
    {
        dirty = false;

        std::vector<bool> boardTemp;
        for (auto i = 0; i < boardSolved.size(); ++i)
        {
            if (!boardSolved[i])
            {
                boardTemp.push_back(false);
                continue;
            }

            const auto leftEdge = (i % width) == 0;
            const auto rightEdge = (i + 1) % width == 0;
            const auto topEdge = i < width;
            const auto bottomEdge = (boardSolved.size() - width) < i;

            const auto left = leftEdge || !boardSolved[i - 1];
            const auto right = rightEdge || !boardSolved[i + 1];
            const auto top = topEdge || !boardSolved[i - width];
            const auto bottom = bottomEdge || !boardSolved[i + width];

            const auto topLeft = leftEdge || topEdge || !boardSolved[i - width - 1];
            const auto topRight = rightEdge || topEdge || !boardSolved[i - width + 1];
            const auto bottomLeft = leftEdge || bottomEdge || !boardSolved[i + width - 1];
            const auto bottomRight = rightEdge || bottomEdge || !boardSolved[i + width + 1];

            int count{};
            if (left) ++count;
            if (right) ++count;
            if (top) ++count;
            if (bottom) ++count;

            if (topLeft) ++count;
            if (topRight) ++count;
            if (bottomLeft) ++count;
            if (bottomRight) ++count;

            // Less than 4 directions should be blocked.  aka at least 5 should be free
            if (count >= 5)
            {
                // Push a false because this is now removed
                boardTemp.push_back(false);
                dirty = true;
            }
            else
            {
                boardTemp.push_back(true);
            }
        }

        std::swap(boardTemp, boardSolved);
    }

    int beginCount = std::accumulate(board.begin(), board.end(), 0, [](int sum, bool val) { return val ? (sum + 1) : sum; });
    int endCount = std::accumulate(boardSolved.begin(), boardSolved.end(), 0, [](int sum, bool val) { return val ? (sum + 1) : sum; });
    std::cout << (beginCount - endCount) << std::endl;
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
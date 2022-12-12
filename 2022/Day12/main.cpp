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

namespace
{
    using point_t = std::pair<int32_t, int32_t>;
    constexpr int32_t c_empty{9999};

    void MoveRecursive(const vector<char>& board, vector<int32_t>& results, int32_t width, int32_t height, point_t currentPos, int32_t counter)
    {
        if (currentPos.first < 0 || currentPos.first >= width || currentPos.second < 0 || currentPos.second >= height) return;

        auto currentHeight = board[currentPos.second * width + currentPos.first];
        const auto movableHeight = currentHeight + 1;
        const auto nextCounter = ++counter;

        point_t up = currentPos;
        up.second--;
        if (up.second >= 0)
        {
            const auto index = up.second * width + up.first;
            if ((board[index] <= movableHeight) && (nextCounter < results[index]))
            {
                results[index] = nextCounter;
                MoveRecursive(board, results, width, height, up, counter);
            }
        }

        point_t down = currentPos;
        down.second++;
        if (down.second < height)
        {
            const auto index = down.second * width + down.first;
            if ((board[index] <= movableHeight) && (nextCounter < results[index]))
            {
                results[index] = nextCounter;
                MoveRecursive(board, results, width, height, down, counter);
            }
        }

        point_t left = currentPos;
        left.first--;
        if (left.first >= 0)
        {
            const auto index = left.second * width + left.first;
            if ((board[index] <= movableHeight) && (nextCounter < results[index]))
            {
                results[index] = nextCounter;
                MoveRecursive(board, results, width, height, left, counter);
            }
        }

        point_t right = currentPos;
        right.first++;
        if (right.first < width)
        {
            const auto index = right.second * width + right.first;
            if ((board[index] <= movableHeight) && (nextCounter < results[index]))
            {
                results[index] = nextCounter;
                MoveRecursive(board, results, width, height, right, counter);
            }
        }
    }

    void Part1()
    {
        vector<char> board;

        std::string input;
        int32_t width{};
        int32_t height{};
        while (std::getline(std::cin, input))
        {
            std::copy(input.begin(), input.end(), back_inserter(board));
            ++height;
            width = input.length();
        }

        point_t start{};
        point_t end{};
        for (auto y = 0; y < height; ++y)
        {
            for (auto x = 0; x < width; ++x)
            {
                if (board[y * width + x] == 'S')
                {
                    start = make_pair(x, y);
                }
                else if (board[y * width + x] == 'E')
                {
                    end = make_pair(x, y);
                }
            }
        }

        board[start.second * width + start.first] = 'a';
        board[end.second * width + end.first] = 'z';
        vector<int32_t> results(board.size(), c_empty);
        MoveRecursive(board, results, width, height, start, 0);
        std::cout << results[end.second * width + end.first] << std::endl;
    }

    void Part2()
    {
        vector<char> board;

        std::string input;
        int32_t width{};
        int32_t height{};
        while (std::getline(std::cin, input))
        {
            std::copy(input.begin(), input.end(), back_inserter(board));
            ++height;
            width = input.length();
        }

        point_t start{};
        point_t end{};
        for (auto y = 0; y < height; ++y)
        {
            for (auto x = 0; x < width; ++x)
            {
                if (board[y * width + x] == 'S')
                {
                    start = make_pair(x, y);
                }
                else if (board[y * width + x] == 'E')
                {
                    end = make_pair(x, y);
                }
            }
        }

        board[start.second * width + start.first] = 'a';
        board[end.second * width + end.first] = 'z';

        std::vector<uint32_t> possibilities;
        for (auto y = 0; y < height; ++y)
        {
            for (auto x = 0; x < width; ++x)
            {
                if (board[y * width + x] == 'a')
                {
                    start = make_pair(x, y);
                    vector<int32_t> results(board.size(), c_empty);
                    MoveRecursive(board, results, width, height, start, 0);

                    const auto possibility{results[end.second * width + end.first]};
                    if (possibility != c_empty)
                    {
                        possibilities.emplace_back(possibility);
                    }
                }
            }
        }

        std::cout << *std::min_element(possibilities.begin(), possibilities.end()) << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
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
    std::string board;

    std::string input;
    int height{};
    int width{};
    while (std::getline(std::cin, input))
    {
        std::copy(input.begin(), input.end(), std::back_inserter(board));
        width = input.size();
        ++height;
    }

    uint64_t result{};

    for (auto line = 0; line < height; ++line)
    {
        for (auto col = 0; col < width; ++col)
        {
            const auto index = line * width + col;
            if (!std::isdigit(board[index]))
            {
                continue;
            }

            // const auto dot = board.find('.', index); // failed if there is a symbol separating numbers
            const auto nonNumIter = std::find_if(board.begin() + index, board.end(),
                [](char c) { return !std::isdigit(c); });

            const int dot = (int)(nonNumIter - board.begin());
            const auto numWidth = dot - index;

            bool valid = false;
            for (auto i = line - 1; (i <= line + 1) && !valid; ++i)
            {
                for (auto j = col - 1; (j <= (col + numWidth)) && !valid; ++j)
                {
                    if ((i < 0) || (i >= height) || (j < 0) || (j >= width))
                    {
                        continue;
                    }

                    const auto innerIndex = i * width + j;
                    if ((board[innerIndex] != '.') && !std::isdigit(board[innerIndex]))
                    {
                        valid = true;
                    }
                }
            }

            if (valid)
            {
                const auto numStr = (dot != string::npos) ? board.substr(index, dot) : board.substr(index);
                const auto num = std::atoi(numStr.c_str());
                // std::cerr << num << std::endl;
                result += num;
            }

            col += numWidth;
        }
    }

    std::cout << result << std::endl;
}

int CrawlNumber(std::string const& board, int width, int height, int x, int y)
{
    // int index = y * width + x;

    auto begin = x;
    while (begin > 0)
    {
        const int trial = y * width + begin - 1;
        if (!isdigit(board[trial]))
        {
            break;
        }
        --begin;
    }

    auto end = x;
    while (end < (width - 1))
    {
        const int trial = y * width + end + 1;
        if (!isdigit(board[trial]))
        {
            break;
        }
        ++end;
    }

    const auto beginIndex = y * width + begin;
    const auto endIndex = y * width + end;
    const auto substr = board.substr(beginIndex, endIndex - beginIndex + 1);
    return std::atoi(substr.c_str());
}

void Part2()
{
    std::string board;

    std::string input;
    int height{};
    int width{};
    while (std::getline(std::cin, input))
    {
        std::copy(input.begin(), input.end(), std::back_inserter(board));
        width = input.size();
        ++height;
    }

    uint64_t result{};

    for (auto line = 0; line < height; ++line)
    {
        for (auto col = 0; col < width; ++col)
        {
            const auto index = line * width + col;
            if (board[index] != '*')
            {
                continue;
            }

            std::unordered_set<int> numbers;
            for (auto i = line - 1; i <= line + 1; ++i)
            {
                for (auto j = col - 1; j <= (col + 1); ++j)
                {
                    if ((i < 0) || (i >= height) || (j < 0) || (j >= width))
                    {
                        continue;
                    }

                    const auto innerIndex = i * width + j;
                    if (std::isdigit(board[innerIndex]))
                    {
                        const auto num = CrawlNumber(board, width, height, j, i);
                        numbers.emplace(num);
                    }
                }
            }

            if (numbers.size() == 2)
            {
                auto temp{1};
                for (const auto tempNum : numbers)
                {
                    // std::cerr << tempNum << std::endl;
                    temp *= tempNum;
                }
                result += temp;
            }
        }
    }

    std::cout << result << std::endl;
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
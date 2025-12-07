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
#include <iomanip>

using namespace std; // I wouldn't normally do this
using namespace std::literals;

#pragma warning (disable: 4267) // size_t <-> uint
#pragma warning (disable: 4189) // unused variable
#pragma warning (disable: 4100) // unused param

enum class Type
{
    Empty,
    Splitter,
    Beam
};

void Part1()
{
    std::string input;

    std::getline(std::cin, input);
    const auto startPos = input.find('S');
    const auto width = input.size();

    std::vector<Type> board;

    while (std::getline(std::cin, input))
    {
        for(char c : input)
        {
            board.emplace_back(c == '^' ? Type::Splitter : Type::Empty);
        }
    }

    // Simulate beam
    const auto height = board.size() / width;
    board[startPos] = Type::Beam;
    for (auto i = 0; i < height - 1; ++i)
    {
        for (auto j = 0; j < width; ++j)
        {
            if (board[i * width + j] == Type::Beam)
            {
                if (board[(i+1) * width + j] == Type::Splitter)
                {
                    if (j != 0)
                    {
                        board[(i+1) * width + (j-1)] = Type::Beam;
                    }
                    
                    if (j != (width-1))
                    {
                        board[(i+1) * width + (j+1)] = Type::Beam;
                    }
                }
                else
                {
                    board[(i+1) * width + j] = Type::Beam;
                }
            }
        }
    }
    
    // Print the final board for debugging purposes
    for (auto i = 0; i < board.size(); ++i)
    {
        // std::cerr << (board[i] == Type::Beam ? '|' : (board[i] == Type::Splitter ? '^' : '.'));
        if (((i+1) % width) == 0)
        {
            // std::cerr << std::endl;
        }
    }

    // Compute how many splits occurred
    int result{};
    for (auto i = 0; i < board.size(); ++i)
    {
        if ((board[i] == Type::Splitter) && (board[i-width] == Type::Beam))
        {
            ++result;
        }
    }

    std::cout << result << std::endl;
}

uint64_t ProbeDownwards(std::vector<Type> const& board, std::vector<uint64_t> const& poss, size_t width, const int x, const int y)
{
    const auto height = board.size() / width;

    uint64_t leftResult{};
    if (x != 0) // probe left
    {
        const auto left = x - 1;
        for (auto i = y + 1; i < height; ++i)
        {
            if (board[i * width + left] == Type::Splitter)
            {
                leftResult = poss[i * width + left];
                break;
            }
        }

        if (leftResult == 0)
        {
            leftResult = poss[(height - 1) * width + left];
        }
    }

    uint64_t rightResult{};
    if (x + 1 != width)
    {
        const auto right = x + 1;
        for (auto i = y + 1; i < height; ++i)
        {
            if (board[i * width + right] == Type::Splitter)
            {
                rightResult = poss[i * width + right];
                break;
            }
        }

        if (rightResult == 0)
        {
            rightResult = poss[(height - 1) * width + right];
        }
    }

    return (leftResult + rightResult);
}

void Part2()
{
    std::string input;

    std::getline(std::cin, input);
    const auto startPos = input.find('S');
    const auto width = input.size();

    std::vector<Type> board;

    while (std::getline(std::cin, input))
    {
        for(char c : input)
        {
            board.emplace_back(c == '^' ? Type::Splitter : Type::Empty);
        }
    }
    
    const auto height = board.size() / width;
    std::vector<uint64_t> poss(board.size(), 0);

    // Seed the lowest row of possibilities to bootstrap the computation.  This needs to be the empty
    // bottom row instead of the lowest row of splitters to account for beams that reach the bottom without
    // hitting a splitter on the way.
    for (auto x = 0; x < width; ++x)
    {
        const auto y = height - 1;
        poss[y * width + x] = 1;
    }

    for (auto y = height - 2; y > 0; --y)
    {
        for (auto x = 0; x < width; ++x)
        {
            if (board[y*width + x] == Type::Splitter)
            {
                const auto possDownward = ProbeDownwards(board, poss, width, x, y);
                poss[y*width + x] = possDownward;
            }
        }
    }

    uint64_t result{};
    for (auto y = 0; y < height; ++y)
    {
        if (board[y * width + startPos] == Type::Splitter)
        {
            result = poss[y * width + startPos];
            break;
        }
    }

    // Print the final board for debugging purposes
    for (auto i = 0; i < poss.size(); ++i)
    {
        // std::cerr << std::setw(3) << std::setfill(' ') << poss[i];
        if (((i+1) % width) == 0)
        {
            // std::cerr << std::endl;
        }
    }

    std::cout << result << std::endl;
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
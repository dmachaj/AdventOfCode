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

void RollNorth(std::vector<std::string>& board)
{
    const auto width = board[0].size();
    for (auto col = 0; col < width; ++col)
    {
        for (auto row = 0; row < board.size(); ++row)
        {
            if (board[row][col] == '.')
            {
                for (auto j = row + 1; j < board.size(); ++j)
                {
                    if (board[j][col] == '#') break;
                    if (board[j][col] == 'O')
                    {
                        std::swap(board[row][col], board[j][col]);
                        break;
                    }
                }
            }
        }
    }
}

void RollSouth(std::vector<std::string>& board)
{
    const auto width = board[0].size();
    for (auto col = 0; col < width; ++col)
    {
        for (int row = board.size() - 1; row >= 0; --row)
        {
            if (board[row][col] == '.')
            {
                for (auto j = row - 1; j >= 0; --j)
                {
                    if (board[j][col] == '#') break;
                    if (board[j][col] == 'O')
                    {
                        std::swap(board[row][col], board[j][col]);
                        break;
                    }
                }
            }
        }
    }
}

void RollWest(std::vector<std::string>& board)
{
    const auto width = board[0].size();
    for (auto row = 0; row < board.size(); ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            if (board[row][col] == '.')
            {
                for (auto j = col + 1; j < width; ++j)
                {
                    if (board[row][j] == '#') break;
                    if (board[row][j] == 'O')
                    {
                        std::swap(board[row][col], board[row][j]);
                        break;
                    }
                }
            }
        }
    }
}

void RollEast(std::vector<std::string>& board)
{
    const auto width = board[0].size();
    for (auto row = 0; row < board.size(); ++row)
    {
        for (int col = width - 1; col >= 0; --col)
        {
            if (board[row][col] == '.')
            {
                for (auto j = col - 1; j >= 0; --j)
                {
                    if (board[row][j] == '#') break;
                    if (board[row][j] == 'O')
                    {
                        std::swap(board[row][col], board[row][j]);
                        break;
                    }
                }
            }
        }
    }
}

void Part1()
{
    std::vector<std::string> board;

    std::string input;
    while (std::getline(std::cin, input))
    {
        board.emplace_back(std::move(input));
    }

    // roll rocks
    RollNorth(board);

    // compute load
    uint64_t result{};
    for (auto i = 0; i < board.size(); ++i)
    {
        const auto cost = board.size() - i;
        for (auto c : board[i])
        {
            if (c == 'O')
            {
                result += cost;
            }
        }
    }

    std::cout << result << std::endl;
}

void Part2()
{
    std::vector<std::string> board;

    std::string input;
    while (std::getline(std::cin, input))
    {
        board.emplace_back(std::move(input));
    }

    std::vector<std::vector<std::string>> everyBoardEver;

    uint64_t cycleCount{};
    do
    {
        everyBoardEver.emplace_back(board);

        // roll rocks
        RollNorth(board);
        RollWest(board);
        RollSouth(board);
        RollEast(board);
        ++cycleCount;
    } while (std::find(everyBoardEver.begin(), everyBoardEver.end(), board) == everyBoardEver.end());

    uint64_t cycleBegin{};
    for (auto i = 0; i < everyBoardEver.size(); ++i)
    {
        if (everyBoardEver[i] == board)
        {
            cycleBegin = i;
            break;
        }
    }
    cycleCount -= cycleBegin;

    std::cerr << "Cycle found between iteration " << cycleBegin << " and " << cycleBegin + cycleCount <<
        " with " << cycleCount << " cycles between them" << std::endl;

    constexpr uint64_t c_cycles{1000000000};
    const auto remainingCycles = (c_cycles - cycleBegin) % cycleCount;

    for (auto i = 0; i < remainingCycles; ++i)
    {
        // roll rocks
        RollNorth(board);
        RollWest(board);
        RollSouth(board);
        RollEast(board);
    }
    
    // compute load
    uint64_t result{};
    for (auto i = 0; i < board.size(); ++i)
    {
        const auto cost = board.size() - i;
        for (auto c : board[i])
        {
            if (c == 'O')
            {
                result += cost;
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
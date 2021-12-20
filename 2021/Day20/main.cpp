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

struct PairHash
{
    size_t operator()(const std::pair<int32_t, int32_t>& pt) const noexcept
    {
        auto h1 = std::hash<int64_t>{}(pt.first);
        auto h2 = std::hash<int64_t>{}(pt.second);
        return h1 ^ (h2 << 1);
    }
};

namespace
{
    uint64_t BinToInt(string_view input)
    {
        uint64_t result{};
        for (auto i = 0; i < input.size(); ++i)
        {
            if (input[i] == '1')
            {
                result += (1ULL << (input.size() - i - 1ULL));
            }
        }
        return result;
    }

    using board_t = unordered_map<std::pair<int32_t, int32_t>, bool, PairHash>;

    // Stupid hard-coded dimensions.  Give the 100x100 unique input space to grow one per iteration for 50 iterations in all directions.
    constexpr auto c_yMin = -60L;
    constexpr auto c_yMax = 160L;
    constexpr auto c_xMin = -60L;
    constexpr auto c_xMax = 160L;

    void PrintBoard(const board_t& board)
    {
        for (auto y = c_yMin; y <= c_yMax; ++y)
        {
            for (auto x = c_xMin; x <= c_xMax; ++x)
            {
                const auto square = board.find(make_pair(x, y))->second;
                std::cerr << (square ? "#" : ".");
            }
            std::cerr << std::endl;
        }
        std::cerr << std::endl;
    }

    std::string BoardSquareToString(const board_t& board, int32_t x, int32_t y)
    {
        const auto iter = board.find(make_pair(x, y));
        if (iter != board.end())
        {
            if (iter->second)
            {
                return "1";
            }
            else
            {
                return "0";
            }
        }
        return "0";
    }

    board_t RunAlgorithm(const board_t& board, const std::string& algo)
    {
        board_t board2;
        for (auto y = c_yMin; y <= c_yMax; ++y)
        {
            for (auto x = c_xMin; x <= c_xMax; ++x)
            {
                string bin;
                bin += BoardSquareToString(board, x - 1, y - 1);
                bin += BoardSquareToString(board, x, y - 1);
                bin += BoardSquareToString(board, x + 1, y - 1);

                bin += BoardSquareToString(board, x - 1, y);
                bin += BoardSquareToString(board, x, y);
                bin += BoardSquareToString(board, x + 1, y);
                
                bin += BoardSquareToString(board, x - 1, y + 1);
                bin += BoardSquareToString(board, x, y + 1);
                bin += BoardSquareToString(board, x + 1, y + 1);

                const uint64_t result = BinToInt(bin);
                const bool algoResult = (algo[result] == '#');
                board2[make_pair(x, y)] = algoResult;
            }
        }

        // Stupid hack!
        // The instability of the infinite surface and our limited view of it means that every other iteration
        // the unknown space toggles between lit and dark.  However, the left/right edges treat space beyond
        // the border as dark so the left/right edges wrongly stay lit instead of alternating like every other
        // square.  Hack around this by detecting the right edge being different from the column next to it and
        // if so make both edges dark to match.
        if (board2.find(make_pair(c_xMax - 1, 0))->second != board2.find(make_pair(c_xMax, 0))->second)
        {
            for (auto y = c_yMin; y <= c_yMax; ++y)
            {
                board2[make_pair(c_xMin, y)] = false;
                board2[make_pair(c_xMax, y)] = false;
            }
        }

        return board2;
    }

    uint64_t RunIterations(const uint32_t iterationCount)
    {
        std::string algo;
        std::getline(std::cin, algo);
        std::cin.get(); // eat newline 

        int32_t yInput{};
        board_t board;

        std::string input;
        while (std::getline(std::cin, input))
        {
            for (auto i = 0UL; i < input.size(); ++i)
            {
                board[make_pair(i, yInput)] = (input[i] == '#');
            }
            ++yInput;
        }

        for (auto i = 0UL; i < iterationCount; ++i)
        {
            board = RunAlgorithm(board, algo);
        }
        // PrintBoard(board);

        const auto result = std::accumulate(board.begin(), board.end(), 0UL,
            [](uint32_t sum, const auto& pos)
            {
                if (pos.second) return sum + 1;
                else return sum;
            });

        return result;
    }

    void Part1()
    {
        const auto result = RunIterations(2);
        std::cout << result << std::endl;
    }

    void Part2()
    {
        const auto result = RunIterations(50);
        std::cout << result << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
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

    void PrintBoard(const board_t& board)
    {
        auto yMin = std::min_element(board.begin(), board.end(), [](const auto& left, const auto& right) { return left.first.second < right.first.second; })->first.second;
        auto yMax = std::max_element(board.begin(), board.end(), [](const auto& left, const auto& right) { return left.first.second < right.first.second; })->first.second;
        auto xMin = std::min_element(board.begin(), board.end(), [](const auto& left, const auto& right) { return left.first.first < right.first.first; })->first.first;
        auto xMax = std::max_element(board.begin(), board.end(), [](const auto& left, const auto& right) { return left.first.first < right.first.first; })->first.first;

        for (auto y = yMin; y <= yMax; ++y)
        {
            for (auto x = xMin; x <= xMax; ++x)
            {
                std::cerr << (board.find(make_pair(x, y))->second ? "#" : ".");
            }
            std::cerr << std::endl;
        }
        std::cerr << std::endl;
    }

    board_t RunAlgorithm(const board_t& board, const std::string& algo, int32_t startWidth, int32_t startHeight)
    {
        auto yMin = std::min_element(board.begin(), board.end(), [](const auto& left, const auto& right) { return left.first.second < right.first.second; })->first.second;
        auto yMax = std::max_element(board.begin(), board.end(), [](const auto& left, const auto& right) { return left.first.second < right.first.second; })->first.second;
        auto xMin = std::min_element(board.begin(), board.end(), [](const auto& left, const auto& right) { return left.first.first < right.first.first; })->first.first;
        auto xMax = std::max_element(board.begin(), board.end(), [](const auto& left, const auto& right) { return left.first.first < right.first.first; })->first.first;

        board_t board2;
        for (auto y = yMin - 1; y <= yMax + 1; ++y)
        {
            for (auto x = xMin - 1; x <= xMax + 1; ++x)
            {
                string bin;
                (board.find(make_pair(x - 1, y - 1)) != board.end() && board.find(make_pair(x - 1, y - 1))->second) ? bin += "1" : bin += "0";
                (board.find(make_pair(x, y - 1)) != board.end() && board.find(make_pair(x, y - 1))->second) ? bin += "1" : bin += "0";
                (board.find(make_pair(x + 1, y - 1)) != board.end() && board.find(make_pair(x + 1, y - 1))->second) ? bin += "1" : bin += "0";

                (board.find(make_pair(x - 1, y)) != board.end() && board.find(make_pair(x - 1, y))->second) ? bin += "1" : bin += "0";
                (board.find(make_pair(x, y)) != board.end() && board.find(make_pair(x, y))->second) ? bin += "1" : bin += "0";
                (board.find(make_pair(x + 1, y)) != board.end() && board.find(make_pair(x + 1, y))->second) ? bin += "1" : bin += "0";

                (board.find(make_pair(x - 1, y + 1)) != board.end() && board.find(make_pair(x - 1, y + 1))->second) ? bin += "1" : bin += "0";
                (board.find(make_pair(x, y + 1)) != board.end() && board.find(make_pair(x, y + 1))->second) ? bin += "1" : bin += "0";
                (board.find(make_pair(x + 1, y + 1)) != board.end() && board.find(make_pair(x + 1, y + 1))->second) ? bin += "1" : bin += "0";

                const uint64_t result = BinToInt(bin);
                const bool algoResult = (algo[result] == '#');
                board2[make_pair(x, y)] = algoResult;
            }
        }
        return board2;
    }

    void Part1()
    {
        std::string algo;
        std::getline(std::cin, algo);
        std::cin.get(); // eat newline 

        int32_t yInput{};
        int32_t xInput{};
        unordered_map<std::pair<int32_t, int32_t>, bool, PairHash> board;

        std::string input;
        while (std::getline(std::cin, input))
        {
            for (auto i = 0UL; i < input.size(); ++i)
            {
                board[make_pair(i, yInput)] = input[i] == '#';
            }
            ++yInput;

            xInput = input.size();
        }

        PrintBoard(board);
        board = RunAlgorithm(board, algo, xInput, yInput);
        PrintBoard(board);
        board = RunAlgorithm(board, algo, xInput, yInput);
        PrintBoard(board);

        const auto result = std::accumulate(board.begin(), board.end(), 0UL,
            [](uint32_t sum, const auto& pos)
            {
                if (pos.second) return sum + 1;
                else return sum;
            });

        std::cout << result << std::endl;
    }

    void Part2()
    {
        std::cout << 0 << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
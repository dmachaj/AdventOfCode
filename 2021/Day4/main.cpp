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

namespace
{
    struct Board
    {
        Board()
        {
            for (auto i = 0UL; i < 25; ++i)
            {
                uint32_t value;
                std::cin >> value;
                data[i] = make_pair(value, false);
            }
            std::cin.get(); // eat newline 
        }

        void MarkNumber(uint32_t number)
        {
            for (auto& val : data)
            {
                if (val.first == number)
                {
                    val.second = true;
                    break;
                }
            }
        }

        bool Winner() const
        {
            // Horizontal
            if (data[0].second && data[1].second && data[2].second && data[3].second && data[4].second) return true;
            if (data[5].second && data[6].second && data[7].second && data[8].second && data[9].second) return true;
            if (data[10].second && data[11].second && data[12].second && data[13].second && data[14].second) return true;
            if (data[15].second && data[16].second && data[17].second && data[18].second && data[19].second) return true;
            if (data[20].second && data[21].second && data[22].second && data[23].second && data[24].second) return true;
            
            // Vertical
            if (data[0].second && data[5].second && data[10].second && data[15].second && data[20].second) return true;
            if (data[1].second && data[6].second && data[11].second && data[16].second && data[21].second) return true;
            if (data[2].second && data[7].second && data[12].second && data[17].second && data[22].second) return true;
            if (data[3].second && data[8].second && data[13].second && data[18].second && data[23].second) return true;
            if (data[4].second && data[9].second && data[14].second && data[19].second && data[24].second) return true;

            return false;
        }

        uint32_t SumUnmarked()
        {
            uint32_t total{};
            for (const auto& val : data)
            {
                if (!val.second)
                {
                    total += val.first;
                }
            }
            return total;
        }

        std::array<std::pair<uint32_t, bool>, 25> data;
    };

    void Part1()
    {
        std::vector<uint32_t> numbers;

        std::string input;
        std::getline(std::cin, input);
        std::istringstream instream(input);
        while (std::getline(instream, input, ','))
        {
            numbers.emplace_back(std::atoi(input.c_str()));
        }

        std::getline(std::cin, input); // blank line

        std::vector<Board> boards;
        while (!std::cin.eof())
        {
            Board board; // constructor reads from stdin
            boards.emplace_back(std::move(board));
            std::getline(std::cin, input); // blank line
        }

        uint32_t winningNumber{};
        Board* winner = nullptr;
        for (auto iter = numbers.begin(); iter != numbers.end() && (winner == nullptr); ++iter)
        {
            for (auto& board : boards)
            {
                board.MarkNumber(*iter);
                if (board.Winner())
                {
                    winner = &board;
                    winningNumber = *iter;
                    break;
                }
            }
        }

        std::cout << (winningNumber * winner->SumUnmarked()) << std::endl;
    }

    void Part2()
    {
        std::vector<uint32_t> numbers;

        std::string input;
        std::getline(std::cin, input);
        std::istringstream instream(input);
        while (std::getline(instream, input, ','))
        {
            numbers.emplace_back(std::atoi(input.c_str()));
        }

        std::getline(std::cin, input); // blank line

        std::vector<Board> boards;
        while (!std::cin.eof())
        {
            Board board; // constructor reads from stdin
            boards.emplace_back(std::move(board));
            std::getline(std::cin, input); // blank line
        }

        uint32_t winningNumber{};
        std::vector<Board*> winners;
        for (auto iter = numbers.begin(); iter != numbers.end() && (winners.size() != boards.size()); ++iter)
        {
            for (auto& board : boards)
            {
                if (board.Winner())
                {
                    continue;
                }

                board.MarkNumber(*iter);
                if (board.Winner())
                {
                    winners.emplace_back(&board);
                    winningNumber = *iter;
                    if (winners.size() == boards.size())
                    {
                        break;
                    }
                }
            }
        }

        std::cout << (winningNumber * winners[winners.size() - 1]->SumUnmarked()) << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
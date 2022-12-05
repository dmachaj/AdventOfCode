#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <stack>
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
    void TryReadLetter(std::string input, int offset, std::stack<char>& col)
    {
        if ((offset < input.length()) && (input[offset] != ' '))
        {
            col.push(input[offset]);
        }
    }

    std::vector<std::stack<char>> ReadBoard()
    {
        std::vector<std::stack<char>> board(9);

        std::string input;
        while (std::getline(std::cin, input))
        {
            if (input[1] == '1')
            {
                std::getline(std::cin, input); // eat blank line
                break;
            }

            TryReadLetter(input, 1, board[0]);
            TryReadLetter(input, 5, board[1]);
            TryReadLetter(input, 9, board[2]);
            TryReadLetter(input, 13, board[3]);
            TryReadLetter(input, 17, board[4]);
            TryReadLetter(input, 21, board[5]);
            TryReadLetter(input, 25, board[6]);
            TryReadLetter(input, 29, board[7]);
            TryReadLetter(input, 33, board[8]);
        }

        for (auto& col : board)
        {
            std::stack<char> flipped;
            while (!col.empty())
            {
                flipped.push(col.top());
                col.pop();
            }
            std::swap(col, flipped);
        }

        return board;
    }

    void Part1()
    {
        auto board = ReadBoard();

        std::string input;
        while (std::getline(std::cin, input))
        {
            // move 1 from 2 to 1
            std::istringstream inStream(input);
            std::string move, countstr, fromfiller, fromstr, tofiller, tostr;
            std::getline(inStream, move, ' ');
            std::getline(inStream, countstr, ' ');
            std::getline(inStream, fromfiller, ' ');
            std::getline(inStream, fromstr, ' ');
            std::getline(inStream, tofiller, ' ');
            std::getline(inStream, tostr, ' ');

            const auto count = std::atoi(countstr.c_str());
            const auto from = std::atoi(fromstr.c_str()) - 1;
            const auto to = std::atoi(tostr.c_str()) - 1;

            for (auto i = 0; i < count; ++i)
            {
                char temp = board[from].top();
                board[from].pop();
                board[to].push(temp);
            }
        }

        for (const auto& col : board)
        {
            if (!col.empty())
            {
                std::cout << col.top();
            }
        }
        std::cout << std::endl;
    }

    void Part2()
    {
        std::vector<std::stack<char>> board = ReadBoard();

        std::string input;

        while (std::getline(std::cin, input))
        {
            // move 1 from 2 to 1
            std::istringstream inStream(input);
            std::string move, countstr, fromfiller, fromstr, tofiller, tostr;
            std::getline(inStream, move, ' ');
            std::getline(inStream, countstr, ' ');
            std::getline(inStream, fromfiller, ' ');
            std::getline(inStream, fromstr, ' ');
            std::getline(inStream, tofiller, ' ');
            std::getline(inStream, tostr, ' ');

            const auto count = std::atoi(countstr.c_str());
            const auto from = std::atoi(fromstr.c_str()) - 1;
            const auto to = std::atoi(tostr.c_str()) - 1;

            std::vector<char> temp;
            for (auto i = 0; i < count; ++i)
            {
                temp.emplace_back(board[from].top());
                board[from].pop();
            }
            std::reverse(temp.begin(), temp.end());

            for (auto c : temp)
            {
                board[to].push(c);
            }
        }

        for (const auto& col : board)
        {
            if (!col.empty())
            {
                std::cout << col.top();
            }
        }
        std::cout << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
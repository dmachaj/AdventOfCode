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
    struct Position
    {
        Position(const string& input, char id):
            identifier(id)
        {
            const auto first = input.substr(0, input.find(','));
            const auto second = input.substr(input.find(',') + 1);
            x = std::atoi(first.c_str());
            y = std::atoi(second.c_str());
        }

        uint32_t DistanceTo(uint32_t otherX, uint32_t otherY) const
        {
            return std::abs((int32_t)x - (int32_t)otherX) + std::abs((int32_t)y - (int32_t)otherY);
        }

        uint32_t x;
        uint32_t y;
        char identifier;
    };

    struct Node
    {
        char MinUniquePosition() const
        {
            const auto min = std::min_element(distanceToPositions.begin(), distanceToPositions.end(),
                [](const auto& left, const auto& right)
                {
                    return left.second < right.second;
                });
            
            const auto allMins = std::count_if(distanceToPositions.begin(), distanceToPositions.end(),
                [min{min->second}](const auto& value)
                {
                    return value.second == min;
                });
            
            if (allMins != 1)
            {
                return '.';
            }
            return min->first;
        }

        uint32_t SumAllDistances() const
        {
            return std::accumulate(distanceToPositions.begin(), distanceToPositions.end(), 0UL, [](uint32_t sum, const auto& value) { return sum + value.second; });
        }
        
        std::unordered_map<char, uint32_t> distanceToPositions;
    };

    vector<Position> ReadInput()
    {
        vector<Position> positions;

        char id = 'A';
        std::string input;
        while (std::getline(std::cin, input))
        {
            positions.emplace_back(Position{input, id});
            ++id;
        }

        return positions;
    }

    void Part1()
    {
        size_t boardSize;
        std::cin >> boardSize;
        std::cin.get(); // eat newline

        auto positions = ReadInput();

        vector<vector<Node>> board;
        board.reserve(boardSize);
        for (auto outer = 0UL; outer < boardSize; ++outer)
        {
            vector<Node> row;
            for (auto inner = 0UL; inner < boardSize; ++inner)
            {
                row.emplace_back(Node());
            }
            board.emplace_back(move(row));
        }

        for (auto y = 0UL; y < boardSize; ++y)
        {
            for (auto x = 0UL; x < boardSize; ++x)
            {
                for (const auto& position : positions)
                {
                    board[x][y].distanceToPositions[position.identifier] = position.DistanceTo(x, y);
                }
            }
        }

        std::unordered_set<char> finitePositions;
        for (const auto& position : positions)
        {
            finitePositions.emplace(position.identifier);
        }

        // Any letter that is minimum at the edge of the board is guaranteed infinite.
        for (auto i = 0UL; i < boardSize; ++i)
        {
            auto id = board[i][0].MinUniquePosition();
            if (finitePositions.find(id) != finitePositions.end())
            {
                finitePositions.erase(id);
            }

            id = board[i][boardSize - 1].MinUniquePosition();
            if (finitePositions.find(id) != finitePositions.end())
            {
                finitePositions.erase(id);
            }

            id = board[0][i].MinUniquePosition();
            if (finitePositions.find(id) != finitePositions.end())
            {
                finitePositions.erase(id);
            }

            id = board[boardSize - 1][i].MinUniquePosition();
            if (finitePositions.find(id) != finitePositions.end())
            {
                finitePositions.erase(id);
            }
        }

        std::map<uint32_t, char> totals;
        for (const auto& pos : finitePositions)
        {
            uint32_t total{};
            for (const auto& outer : board)
            {
                for (const auto& inner : outer)
                {
                    if (inner.MinUniquePosition() == pos)
                    {
                        ++total;
                    }
                }
            }
            totals[total] = pos;
        }

        // for (auto y = 0UL; y < boardSize; ++y)
        // {
        //     for (auto x = 0UL; x < boardSize; ++x)
        //     {
        //         std::cerr << board[x][y].MinUniquePosition();
        //     }
        //     std::cerr << std::endl;
        // }

        std::cout << totals.rbegin()->first << std::endl;
    }

    void Part2()
    {
        size_t boardSize;
        std::cin >> boardSize;
        std::cin.get(); // eat newline

        uint32_t maxDistances;
        std::cin >> maxDistances;
        std::cin.get(); // eat newline

        auto positions = ReadInput();

        vector<vector<Node>> board;
        board.reserve(boardSize);
        for (auto outer = 0UL; outer < boardSize; ++outer)
        {
            vector<Node> row;
            for (auto inner = 0UL; inner < boardSize; ++inner)
            {
                row.emplace_back(Node());
            }
            board.emplace_back(move(row));
        }

        for (auto y = 0UL; y < boardSize; ++y)
        {
            for (auto x = 0UL; x < boardSize; ++x)
            {
                for (const auto& position : positions)
                {
                    board[x][y].distanceToPositions[position.identifier] = position.DistanceTo(x, y);
                }
            }
        }

        uint32_t total{};
        for (auto y = 0UL; y < boardSize; ++y)
        {
            for (auto x = 0UL; x < boardSize; ++x)
            {
                if (board[x][y].SumAllDistances() < maxDistances)
                {
                    ++total;
                }
            }
        }

        std::cout << total << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
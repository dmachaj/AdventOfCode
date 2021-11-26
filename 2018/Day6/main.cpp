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
            std::multiset<uint32_t> distances;
            for (const auto& positions : distanceToPositions)
            {
                distances.emplace(positions.second);
            }

            if (*distances.begin() == *(++distances.begin()))
            {
                return '.';
            }

            for (const auto& positions : distanceToPositions)
            {
                if (positions.second == *distances.begin())
                {
                    return positions.first;
                }
            }

            throw std::exception("wtf");
            return '0';
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
        auto positions = ReadInput();

        constexpr size_t c_boardSize{400};
        array<array<Node, c_boardSize>, c_boardSize> board;

        for (auto y = 0UL; y < c_boardSize; ++y)
        {
            for (auto x = 0UL; x < c_boardSize; ++x)
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
        for (auto i = 0UL; i < c_boardSize; ++i)
        {
            auto id = board[i][0].MinUniquePosition();
            if (finitePositions.find(id) != finitePositions.end())
            {
                finitePositions.erase(id);
            }

            id = board[i][c_boardSize - 1].MinUniquePosition();
            if (finitePositions.find(id) != finitePositions.end())
            {
                finitePositions.erase(id);
            }

            id = board[0][i].MinUniquePosition();
            if (finitePositions.find(id) != finitePositions.end())
            {
                finitePositions.erase(id);
            }

            id = board[c_boardSize - 1][i].MinUniquePosition();
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

        // for (auto y = 0UL; y < c_boardSize; ++y)
        // {
        //     for (auto x = 0UL; x < c_boardSize; ++x)
        //     {
        //         std::cerr << board[x][y].MinUniquePosition();
        //     }
        //     std::cerr << std::endl;
        // }

        std::cout << totals.rbegin()->first << std::endl;
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
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

namespace
{
    std::vector<std::pair<char, uint32_t>> ExtractInput()
    {
        std::vector<std::pair<char, uint32_t>> result{};

        std::string input;
        std::getline(std::cin, input);

        std::istringstream inStream(input);
        std::string operation;
        while (std::getline(inStream, operation, ','))
        {
            char direction = operation[0];
            uint32_t distance = std::atoi(operation.substr(1).c_str());

            // std::cerr << direction << " " << distance << std::endl;
            result.emplace_back(std::make_pair(direction, distance));
        }

        return result;
    }

    template <typename TLambda>
    void PerformMoves(const std::vector<std::pair<char, uint32_t>>& moves, TLambda callback)
    {
        std::pair<int32_t, int32_t> currentLocation{};
        uint32_t moveCount{};
        for (const auto move : moves)
        {
            auto movesRemaining = move.second;
            for (uint32_t i = 0; i < movesRemaining; ++i)
            {
                switch(move.first)
                {
                    case 'U':
                        currentLocation.second++;
                        break;
                    case 'D':
                        currentLocation.second--;
                        break;
                    case 'L':
                        currentLocation.first--;
                        break;
                    case 'R':
                        currentLocation.first++;
                        break;
                    default:
                        throw std::exception("wtf");
                }
                callback(currentLocation, ++moveCount);
            }
        }
    }
}

void Part1()
{
    constexpr uint32_t c_gridDimensions{10000};
    std::vector<std::vector<uint32_t>> grid(c_gridDimensions * 2, std::vector<uint32_t>(c_gridDimensions * 2, 0));

    const auto firstWire = ExtractInput();
    PerformMoves(firstWire, [&](const std::pair<int32_t, int32_t>& currentLocation, uint32_t)
    {
        grid[c_gridDimensions + currentLocation.first][c_gridDimensions + currentLocation.second] = true;
    });

    const auto secondWire = ExtractInput();
    std::vector<std::pair<int32_t, int32_t>> hits{};
    PerformMoves(secondWire, [&](const std::pair<int32_t, int32_t>& currentLocation, uint32_t)
    {
        if (grid[c_gridDimensions + currentLocation.first][c_gridDimensions + currentLocation.second])
        {
            hits.emplace_back(std::make_pair(currentLocation.first, currentLocation.second));
        }
    });

    std::vector<uint32_t> distances{};
    for (const auto hit : hits)
    {
        distances.emplace_back(std::abs(hit.first) + std::abs(hit.second));
    }
    std::sort(distances.begin(), distances.end());

    std::cout << distances[0] << std::endl;
}

void Part2()
{
    constexpr uint32_t c_gridDimensions{10000};
    std::vector<std::vector<uint32_t>> grid(c_gridDimensions * 2, std::vector<uint32_t>(c_gridDimensions * 2, 0));

    const auto firstWire = ExtractInput();
    PerformMoves(firstWire, [&](const std::pair<int32_t, int32_t>& currentLocation, uint32_t moveCount)
    {
        grid[c_gridDimensions + currentLocation.first][c_gridDimensions + currentLocation.second] = moveCount;
    });

    const auto secondWire = ExtractInput();
    std::vector<std::pair<int32_t, int32_t>> hits{};
    PerformMoves(secondWire, [&](const std::pair<int32_t, int32_t>& currentLocation, uint32_t moveCount)
    {
        if (grid[c_gridDimensions + currentLocation.first][c_gridDimensions + currentLocation.second] != 0)
        {
            hits.emplace_back(std::make_pair(grid[c_gridDimensions + currentLocation.first][c_gridDimensions + currentLocation.second], moveCount));
        }
    });

    std::vector<uint32_t> distances{};
    for (const auto hit : hits)
    {
        distances.emplace_back(std::abs(hit.first) + std::abs(hit.second));
    }
    std::sort(distances.begin(), distances.end());

    std::cout << distances[0] << std::endl;
}

int main()
{
    // Part1();
    Part2();
    return 0;
}
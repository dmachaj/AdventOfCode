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
#include "../Intcode/Intcode.h"
#include <fstream>

using namespace std; // I wouldn't normally do this
using namespace std::literals;

#pragma warning (disable: 4267) // size_t <-> uint
#pragma warning (disable: 4189) // unused variable
#pragma warning (disable: 4100) // unused param

namespace
{
    enum class Movement
    {
        North = 1,
        South = 2,
        West = 3,
        East = 4
    };

    enum class StatusCode
    {
        HitWall = 0,
        Moved = 1,
        MovedAndFoundAnswer = 2
    };

    struct Location
    {
        char output{};
        uint32_t distanceFromStart{};
    };

    using Maze = std::map<std::pair<int32_t, int32_t>, Location>;
    using Position = std::pair<int32_t, int32_t>;

    std::pair<int32_t, int32_t> IncrementLocation(const Position& currentLocation, Movement direction)
    {
        auto result = currentLocation;
        switch(direction)
        {
            case Movement::North:
                result.second--;
                break;
            case Movement::South:
                result.second++;
                break;
            case Movement::West:
                result.first--;
                break;
            case Movement::East:
                result.first++;
                break;
        }
        return result;
    }

    void PrintMapToStderr(const Maze& map)
    {
        const auto xCompare = [](const auto& lhs, const auto& rhs) { return lhs.first.first < rhs.first.first; };
        const auto yCompare = [](const auto& lhs, const auto& rhs) { return lhs.first.second < rhs.first.second; };
        auto minX = std::min_element(map.begin(), map.end(), xCompare)->first.first;
        auto maxX = std::max_element(map.begin(), map.end(), xCompare)->first.first;
        auto minY = std::min_element(map.begin(), map.end(), yCompare)->first.second;
        auto maxY = std::max_element(map.begin(), map.end(), yCompare)->first.second;

        for (auto y = minY; y <= maxY; ++y)
        {
            for (auto x = minX; x <= maxX; ++x)
            {
                auto location = std::make_pair(x, y);
                if (map.find(location) != map.end())
                {
                    if (location == std::make_pair(0, 0))
                    {
                        std::cerr << "X";
                    }
                    else
                    {
                        std::cerr << map.at(location).output;
                    }
                }
                else
                {
                    std::cerr << "?";
                }
            }
            std::cerr << std::endl;
        }
        std::cerr << std::endl << std::endl;
    }

    void FloodFillMap(Intcode::ProgramState state, Maze& map, Position currentLocation, uint32_t distanceFromStart)
    {
        if (map.find(currentLocation) != map.end())
        {
            return;
        }
        map[currentLocation] = {'.', distanceFromStart };

        if (distanceFromStart % 30 == 0)
        {
            PrintMapToStderr(map);
        }

        const auto directionLambda = [&](Movement direction)
        {
            auto stateCopy = state;
            auto newLocation = IncrementLocation(currentLocation, direction);
            Intcode::VectorIntcodeInput input({(int64_t)direction});
            Intcode::VectorIntcodeOutput output;
            Intcode::ExecuteProgram(stateCopy, &input, &output);
            StatusCode result = (StatusCode)output.outputs[0];
            if (result == StatusCode::HitWall)
            {
                map[newLocation] = {'#', distanceFromStart + 1 };
            }
            else if (result == StatusCode::MovedAndFoundAnswer)
            {
                // Recurse!
                FloodFillMap(stateCopy, map, newLocation, distanceFromStart + 1);
                map[newLocation] = {'O', distanceFromStart + 1 };
            }
            else if (result == StatusCode::Moved)
            {
                // Recurse!
                FloodFillMap(stateCopy, map, newLocation, distanceFromStart + 1);
            }
        };

        directionLambda(Movement::North);
        directionLambda(Movement::East);
        directionLambda(Movement::South);
        directionLambda(Movement::West);
    }

    void Part1()
    {
        auto state = Intcode::ParseProgram();

        // Run once until it is ready for the first input
        {
            Intcode::VectorIntcodeInput input;
            Intcode::VectorIntcodeOutput output;
            Intcode::ExecuteProgram(state, &input, &output);
        }

        Maze map;
        Position currentLocation{};
        FloodFillMap(state, map, currentLocation, 0UL);
        PrintMapToStderr(map);

        const uint32_t result = std::find_if(map.begin(), map.end(), [](const auto& location) { return location.second.output == 'O'; })->second.distanceFromStart;
        std::cout << result << std::endl;
    }

    void Part2()
    {
        auto state = Intcode::ParseProgram();

        // Run once until it is ready for the first input
        {
            Intcode::VectorIntcodeInput input;
            Intcode::VectorIntcodeOutput output;
            Intcode::ExecuteProgram(state, &input, &output);
        }

        Maze map;
        Position currentLocation{};
        FloodFillMap(state, map, currentLocation, 0UL);
        PrintMapToStderr(map);

        const auto Done = [](const Maze& map)
        {
            return std::find_if(map.begin(), map.end(), [](const auto& location)
            {
                return location.second.output == '.';
            }) == map.end();
        };

        uint32_t iterationCount{};
        while (!Done(map))
        {
            ++iterationCount;

            auto mapCopy = map;
            for (const auto& location : mapCopy)
            {
                if (location.second.output == 'O')
                {
                    auto north = IncrementLocation(location.first, Movement::North);
                    if (mapCopy[north].output == '.') { map[north].output = 'O'; }

                    auto east = IncrementLocation(location.first, Movement::East);
                    if (mapCopy[east].output == '.') { map[east].output = 'O'; }

                    auto south = IncrementLocation(location.first, Movement::South);
                    if (mapCopy[south].output == '.') { map[south].output = 'O'; }

                    auto west = IncrementLocation(location.first, Movement::West);
                    if (mapCopy[west].output == '.') { map[west].output = 'O'; }
                }
            }

            if (iterationCount % 40 == 0)
            {
                PrintMapToStderr(map);
            }
        }

        PrintMapToStderr(map);
        std::cout << iterationCount << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
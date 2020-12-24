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
    enum class Direction
    {
        East,
        SouthEast,
        SouthWest,
        West,
        NorthWest,
        NorthEast
    };

    std::vector<Direction> ParseInput(const std::string& input)
    {
        std::vector<Direction> result;
        for (auto i = 0UL; i < input.size(); ++i)
        {
            if ((i + 1) < input.size())
            {
                if (input[i] == 'n')
                {
                    if (input[i+1] == 'e')
                    {
                        result.emplace_back(Direction::NorthEast);
                        ++i;
                    }
                    else if (input[i+1] == 'w')
                    {
                        result.emplace_back(Direction::NorthWest);
                        ++i;
                    }
                    else
                    {
                        throw;
                    }
                }
                else if (input[i] == 's')
                {
                    if (input[i+1] == 'e')
                    {
                        result.emplace_back(Direction::SouthEast);
                        ++i;
                    }
                    else if (input[i+1] == 'w')
                    {
                        result.emplace_back(Direction::SouthWest);
                        ++i;
                    }
                    else
                    {
                        throw;
                    }
                }
                else if (input[i] == 'e')
                {
                    result.emplace_back(Direction::East);
                }
                else if (input[i] == 'w')
                {
                    result.emplace_back(Direction::West);
                }
                else
                {
                    throw;
                }
            }
            else if (input[i] == 'e')
            {
                result.emplace_back(Direction::East);
            }
            else if (input[i] == 'w')
            {
                result.emplace_back(Direction::West);
            }
            else
            {
                throw;
            }
        }
        return result;
    }

    std::pair<int32_t, int32_t> MoveDirection(std::pair<int32_t, int32_t> currentPosition, Direction command)
    {
        switch (command)
        {
            case Direction::East:
                currentPosition.first++;
                break;
                
            case Direction::West:
                currentPosition.first--;
                break;

            case Direction::SouthEast:
                if (currentPosition.second % 2 != 0)
                {
                    currentPosition.first++;
                }
                currentPosition.second++;
                break;

            case Direction::SouthWest:
                if (currentPosition.second % 2 == 0)
                {
                    currentPosition.first--;
                }
                currentPosition.second++;
                break;

            case Direction::NorthEast:
                if (currentPosition.second % 2 != 0)
                {
                    currentPosition.first++;
                }
                currentPosition.second--;
                break;

            case Direction::NorthWest:
                if (currentPosition.second % 2 == 0)
                {
                    currentPosition.first--;
                }
                currentPosition.second--;
                break;
        }
        return currentPosition;
    }

    void Part1()
    {
        std::vector<std::string> tileLocations;
        {
            std::string input;
            while (std::getline(std::cin, input))
            {
                tileLocations.emplace_back(std::move(input));
            }
        }

        // X,Y coordinate system.  False == white.  True == black.  
        // https://www.redblobgames.com/grids/hexagons/ Uses offset coordinates, odd-r system
        std::map<std::pair<int32_t, int32_t>, bool> coordinateGrid;

        for (const auto& location : tileLocations)
        {
            const auto commands = ParseInput(location);
            std::pair<int32_t, int32_t> currentPosition{};
            for (auto command : commands)
            {
                currentPosition = MoveDirection(currentPosition, command);
            }
            coordinateGrid[currentPosition] = !coordinateGrid[currentPosition];
        }

        const uint32_t result = std::accumulate(coordinateGrid.begin(), coordinateGrid.end(), 0UL, [](uint32_t sum, const std::pair<std::pair<int32_t, int32_t>, bool>& val)
        {
            return val.second ? (sum + 1) : sum;
        });
        std::cout << result << std::endl;
    }

    void Part2()
    {
        std::vector<std::string> tileLocations;
        {
            std::string input;
            while (std::getline(std::cin, input))
            {
                tileLocations.emplace_back(std::move(input));
            }
        }

        // X,Y coordinate system.  False == white.  True == black.  
        // https://www.redblobgames.com/grids/hexagons/ Uses offset coordinates, odd-r system
        std::map<std::pair<int32_t, int32_t>, bool> coordinateGrid;

        for (const auto& location : tileLocations)
        {
            const auto commands = ParseInput(location);
            std::pair<int32_t, int32_t> currentPosition{};
            for (auto command : commands)
            {
                currentPosition = MoveDirection(currentPosition, command);
            }
            coordinateGrid[currentPosition] = !coordinateGrid[currentPosition];
        }

        for (auto i = 0UL; i < 100UL; ++i)
        {
            std::map<std::pair<int32_t, int32_t>, uint32_t> neighborGrid;
            for (const auto& location : coordinateGrid)
            {
                if (!location.second) continue;
                if (neighborGrid.find(location.first) == neighborGrid.end())
                {
                    neighborGrid[location.first] = 0;
                }

                const auto east = MoveDirection(location.first, Direction::East);
                neighborGrid[east]++;
                
                const auto northeast = MoveDirection(location.first, Direction::NorthEast);
                neighborGrid[northeast]++;
                
                const auto northwest = MoveDirection(location.first, Direction::NorthWest);
                neighborGrid[northwest]++;
                
                const auto southeast = MoveDirection(location.first, Direction::SouthEast);
                neighborGrid[southeast]++;
                
                const auto southwest = MoveDirection(location.first, Direction::SouthWest);
                neighborGrid[southwest]++;
                
                const auto west = MoveDirection(location.first, Direction::West);
                neighborGrid[west]++;
            }

            for (const auto& location : neighborGrid)
            {
                if (coordinateGrid[location.first] && ((location.second == 0) || (location.second > 2)))
                {
                    coordinateGrid[location.first] = false;
                }
                else if (!coordinateGrid[location.first] && (location.second == 2))
                {
                    coordinateGrid[location.first] = true;
                }
            }

            // const uint32_t total = std::accumulate(coordinateGrid.begin(), coordinateGrid.end(), 0UL, [](uint32_t sum, const std::pair<std::pair<int32_t, int32_t>, bool>& val)
            // {
            //     return val.second ? (sum + 1) : sum;
            // });
            // std::cerr << "Day " << (i + 1) << ": " << total << std::endl;
        }

        const uint32_t result = std::accumulate(coordinateGrid.begin(), coordinateGrid.end(), 0UL, [](uint32_t sum, const std::pair<std::pair<int32_t, int32_t>, bool>& val)
        {
            return val.second ? (sum + 1) : sum;
        });
        std::cout << result << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
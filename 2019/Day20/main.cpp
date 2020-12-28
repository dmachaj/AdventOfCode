#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include "HelpfulInfrastructure.h"

#pragma warning (disable: 4267) // size_t <-> uint

namespace
{
    struct Board
    {
        std::vector<char> allSquares{};
        uint32_t width{};
        uint32_t height{};
    };

    // struct Portal
    // {
    //     std::string name{};
    //     std::pair<uint32_t, uint32_t> entrance;
    //     std::pair<uint32_t, uint32_t> exit;
    // };

    struct Path
    {
        std::string begin{};
        std::string end{};
        uint32_t distance{};
    };

    bool operator== (const Path& left, const Path& right)
    {
        return (((left.begin == right.begin) && (left.end == right.end)) ||
                ((left.begin == right.end) && (left.end == right.begin))) && (left.distance == right.distance);
    }

    enum class Direction
    {
        Left,
        Right,
        Up,
        Down
    };

    char SquareFromBoard(const Board& board, uint32_t x, uint32_t y)
    {
        return board.allSquares[y * board.width + x];
    }

    std::pair<uint32_t, uint32_t> FindAdjacentDot(const Board& board, uint32_t x, uint32_t y)
    {
        if ((x - 1) < board.width)
        {
            if (SquareFromBoard(board, x - 1, y) == '.')
            {
                return std::make_pair(x - 1, y);
            }
        }

        if ((x + 1) < board.width)
        {
            if (SquareFromBoard(board, x + 1, y) == '.')
            {
                return std::make_pair(x + 1, y);
            }
        }

        if ((y - 1) < board.height)
        {
            if (SquareFromBoard(board, x, y - 1) == '.')
            {
                return std::make_pair(x, y - 1);
            }
        }

        if ((y + 1) < board.height)
        {
            if (SquareFromBoard(board, x, y + 1) == '.')
            {
                return std::make_pair(x, y + 1);
            }
        }

        // Not found
        return std::make_pair(UINT32_MAX, UINT32_MAX);
    }

    std::pair<std::pair<uint32_t, uint32_t>, Direction> FindPortalExit(const Board& board, std::string portalName, uint32_t currentX, uint32_t currentY)
    {
        for (auto y = 0UL; y < board.height; ++y)
        {
            for (auto x = 0UL; x < board.width; ++x)
            {
                if ((x == currentX) && (y == currentY)) { continue; }
                
                const auto currentSquare = SquareFromBoard(board, x, y);
                if (currentSquare == portalName[0])
                {
                    const auto lambda = [&](uint32_t adjacentX, uint32_t adjacentY) -> std::optional<std::pair<std::pair<uint32_t, uint32_t>, Direction>>
                    {
                        if ((adjacentX < board.width) && (adjacentY < board.height))
                        {
                            std::string attempt;
                            attempt.append(1, currentSquare);
                            attempt.append(1, SquareFromBoard(board, adjacentX, adjacentY));
                            if ((attempt == portalName))
                            {
                                const auto adjacent = FindAdjacentDot(board, x, y);
                                if (adjacent.first != UINT32_MAX)
                                {
                                    if (adjacent.first > x)
                                    {
                                        return std::make_pair(adjacent, Direction::Right);
                                    }
                                    else if (adjacent.first < x)
                                    {
                                        return std::make_pair(adjacent, Direction::Left);
                                    }
                                    else if (adjacent.second > y)
                                    {
                                        return std::make_pair(adjacent, Direction::Down);
                                    }
                                    else
                                    {
                                        return std::make_pair(adjacent, Direction::Up);
                                    }
                                }
                                const auto adjacentNext = FindAdjacentDot(board, adjacentX, adjacentY);
                                if (adjacentNext.first == UINT32_MAX) { throw std::exception("wtf");}
                                if (adjacentNext.first > x)
                                {
                                    return std::make_pair(adjacentNext, Direction::Right);
                                }
                                else if (adjacentNext.first < x)
                                {
                                    return std::make_pair(adjacentNext, Direction::Left);
                                }
                                else if (adjacentNext.second > y)
                                {
                                    return std::make_pair(adjacentNext, Direction::Down);
                                }
                                else
                                {
                                    return std::make_pair(adjacentNext, Direction::Up);
                                }
                            }
                        }
                        return {};
                    };

                    if (const auto result = lambda(x-1, y); result)
                    {
                        return result.value();
                    }
                    if (const auto result = lambda(x+1, y); result)
                    {
                        return result.value();
                    }
                    if (const auto result = lambda(x, y-1); result)
                    {
                        return result.value();
                    }
                    if (const auto result = lambda(x, y+1); result)
                    {
                        return result.value();
                    }
                }
            }
        }

        return { { 0, 0}, Direction::Up };
    }

    void GeneratePathRecursively(const Board& board, std::vector<Path>& results, 
        std::string entrance, uint32_t currentX, uint32_t currentY, uint32_t currentDistance, Direction direction)
    {
        // // Detect underflow - note not actually possible given the spacing around the board.
        // if ((currentX > 1000) || (currentY > 1000))
        // {
        //     return {};
        // }

        const auto currentSquare = SquareFromBoard(board, currentX, currentY);
        if (currentSquare == '#' || currentSquare == ' ')
        {
            return; // dead end
        }

        if (currentSquare == '.')
        {
            if (direction != Direction::Right)
            {
                GeneratePathRecursively(board, results, entrance, currentX - 1, currentY, currentDistance + 1, Direction::Left);
            }

            if (direction != Direction::Left) 
            {
                GeneratePathRecursively(board, results, entrance, currentX + 1, currentY, currentDistance + 1, Direction::Right);
            }

            if (direction != Direction::Down)
            {
                GeneratePathRecursively(board, results, entrance, currentX, currentY - 1, currentDistance + 1, Direction::Up);
            }

            if (direction != Direction::Up)
            {
                GeneratePathRecursively(board, results, entrance, currentX, currentY + 1, currentDistance + 1, Direction::Down);
            }

            return;
        }

        if (currentSquare == 'Z')
        {
            Path currentPath { entrance, "ZZ", currentDistance - 1 };
            std::cerr << "Found path " << entrance << "-ZZ: " << currentDistance << std::endl;
            results.push_back(currentPath);
            return;
        }

        if ((currentSquare >= 'A') && (currentSquare <= 'Z'))
        {
            std::pair<uint32_t, uint32_t> nextSquare = { currentX, currentY };
            switch (direction)
            {
                case Direction::Left:
                    nextSquare.first--; break;
                case Direction::Right:
                    nextSquare.first++; break;
                case Direction::Up:
                    nextSquare.second--; break;
                case Direction::Down:
                    nextSquare.second++; break;
            }

            std::string portalName{};
            portalName.append(1, currentSquare);
            portalName.append(1, SquareFromBoard(board, nextSquare.first, nextSquare.second));

            if (portalName == "AA")
            {
                return; // we cycled to the entrance :(
            }

            if (entrance != "AA")
            {
                currentDistance--; // subtract one for double-counted portal costs
            }

            Path currentPath { entrance, portalName, currentDistance };
            if (std::find_if(results.begin(), results.end(), 
                [currentPath](const Path& other)
                {
                    return ((currentPath.begin == other.begin) && (currentPath.end == other.end)) ||
                           ((currentPath.begin == other.end) && (currentPath.end == other.begin));
                }) != results.end())
            {
                return; // we found a cycle
            }

            std::cerr << "Found path " << entrance << "-" << portalName << ": " << currentDistance << std::endl;
            results.push_back(currentPath);
    
            const auto exit = FindPortalExit(board, portalName, currentX, currentY);
            GeneratePathRecursively(board, results, portalName, exit.first.first, exit.first.second, 0, exit.second);
            return;
        }

        return;

        // TEST - hard-coded shim for first sample to exercise graph traversal logic
        // board;startX;startY;
        // Path abbc { "AA", "BC", 4 };
        // Path bcde { "BC", "DE", 6 };
        // Path defg { "DE", "FG", 4 };
        // Path fgzz { "FG", "ZZ", 6 };
        // Path aazz { "AA", "ZZ", 26 };
        // return { abbc, bcde, defg, fgzz, aazz };
    }

    uint32_t FindShortestPathRecursively(const std::vector<Path>& allPaths, std::string currentEntrance, std::vector<Path> visitedSoFar)
    {
        // break recursion if we hit ZZ
        if (currentEntrance == "ZZ")
        {
            return 0;
        }

        std::vector<Path> connections{};
        for (const auto& path : allPaths)
        {
            if (path.begin == currentEntrance)
            {
                connections.push_back(path);
            }
        }

        if (connections.size() == 0)
        {
            return UINT32_MAX;
        }
        
        std::vector<uint32_t> distances{};
        for (const auto& connection : connections)
        {
            if (std::find(visitedSoFar.begin(), visitedSoFar.end(), connection) != visitedSoFar.end())
            {
                continue; // break cycles
            }
            visitedSoFar.push_back(connection);

            const auto recursiveDistance = FindShortestPathRecursively(allPaths, connection.end, visitedSoFar);
            if (recursiveDistance == UINT32_MAX)
            {
                continue; // if there is no path to ZZ from here then disregard the cost
            }

            // If we aren't connecting to ZZ from here then add one step for the portal itself.
            const auto distance = recursiveDistance + connection.distance + ((recursiveDistance != 0) ? 1 : 0);
            distances.emplace_back((distance));

            // for (const auto path : visitedSoFar)
            // {
            //     std::cerr << path.begin << "-" << path.end << " ";
            // }
            // std::cerr << " distance " << distance << std::endl;
        }

        if (distances.size() == 0)
        {
            return UINT32_MAX; // if there is no path to ZZ from here then disregard the cost
        }

        return *std::min_element(distances.begin(), distances.end());
    }

    void Part1()
    {
        std::string input;
        std::vector<char> allSquares{};
        uint32_t width{};
        while (std::getline(std::cin, input))
        {
            width = input.length();
            for (const auto letter : input)
            {
                allSquares.emplace_back(letter);
            }
        }
        const uint32_t height = allSquares.size() / width;

        // TEST - output allSquares
        // std::cerr << "Width = " << width << ". Height = " << height << std::endl;
        // for (auto y = 0UL; y < height; ++y)
        // {
        //     for (auto x = 0UL; x < width; ++x)
        //     {
        //         std::cerr << allSquares[y * width + x];
        //     }
        //     std::cerr << std::endl;
        // }

        std::pair<uint32_t, uint32_t> startingPosition{0, 3};
        for (auto x = 0UL; x < width; ++x)
        {
            if (allSquares[width + x] == 'A')
            {
                startingPosition.first = x;
                break;
            }
        }
        std::cerr << "Starting position is (" << startingPosition.first << "," << startingPosition.second << ")" << std::endl;

        const Board board = { allSquares, width, height };
        std::vector<Path> allPaths{};
        GeneratePathRecursively(board, allPaths, "AA", startingPosition.first, startingPosition.second, 0, Direction::Down);

        // for (const auto& path : allPaths)
        // {
        //     std::cerr << path.begin << " to " << path.end << " distance of " << path.distance << std::endl;
        // }

        std::vector<Path> visitedSoFar{};
        const auto shortestPath = FindShortestPathRecursively(allPaths, "AA", visitedSoFar);
        std::cout << shortestPath << std::endl;
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
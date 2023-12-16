#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
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

char Find(std::string const& board, uint32_t width, uint32_t x, uint32_t y)
{
    const auto index = y * width + x;
    if (index > board.size()) throw std::exception();
    return board[index];
}

struct Beam
{
    int X{};
    int Y{};
    enum class Direction
    {
        Up,
        Down,
        Left,
        Right
    } Dir;

    void Move()
    {
        switch (Dir)
        {
        case Direction::Up:
            --Y;
            break;
        case Direction::Down:
            ++Y;
            break;
        case Direction::Left:
            --X;
            break;
        case Direction::Right:
            ++X;
            break;
        }
    }

    bool IsValid(uint32_t width, uint32_t height)
    {
        if (X < 0 || Y < 0) return false;
        if (X >= (int32_t)width || Y >= (int32_t)height) return false;
        return true;
    }

    bool operator==(const Beam& other) const
    {
        return X == other.X && Y == other.Y && Dir == other.Dir;
    }
};

struct BeamHash
{
    size_t operator()(const Beam& b) const noexcept
    {
        auto h1 = std::hash<int32_t>{}(b.X);
        auto h2 = std::hash<int32_t>{}(b.Y);
        auto h3 = std::hash<int32_t>{}((int32_t)b.Dir);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

uint32_t RunSimulation(std::string const& board, uint32_t width, uint32_t height, Beam firstBeam)
{
    std::vector<Beam> beams;
    beams.emplace_back(std::move(firstBeam));

    std::unordered_set<Beam, BeamHash> beamCache;
    std::vector<bool> visited(board.size(), false);

    while (!beams.empty())
    {
        std::vector<Beam> beam2;

        for (auto b : beams) // copy
        {
            if (beamCache.find(b) != beamCache.end())
            {
                continue; // terminating condition
            }
            beamCache.emplace(b);

            if (b.IsValid(width, height)) // first beam begins at -1 X
            {
                visited[b.Y * width + b.X] = true;
            }

            b.Move();
            if (!b.IsValid(width, height))
            {
                continue;
            }


            const auto value = Find(board, width, b.X, b.Y);
            switch (value)
            {
            case '.':
                beam2.emplace_back(b);
                break;
            case '/':
                if (b.Dir == Beam::Direction::Up) b.Dir = Beam::Direction::Right;
                else if (b.Dir == Beam::Direction::Down) b.Dir = Beam::Direction::Left;
                else if (b.Dir == Beam::Direction::Left) b.Dir = Beam::Direction::Down;
                else if (b.Dir == Beam::Direction::Right) b.Dir = Beam::Direction::Up;
                beam2.emplace_back(b);
                break;
            case '\\':
                if (b.Dir == Beam::Direction::Up) b.Dir = Beam::Direction::Left;
                else if (b.Dir == Beam::Direction::Down) b.Dir = Beam::Direction::Right;
                else if (b.Dir == Beam::Direction::Left) b.Dir = Beam::Direction::Up;
                else if (b.Dir == Beam::Direction::Right) b.Dir = Beam::Direction::Down;
                beam2.emplace_back(b);
                break;
            case '|':
                if (b.Dir == Beam::Direction::Up || b.Dir == Beam::Direction::Down) beam2.emplace_back(b);
                else
                {
                    Beam bCopy = b;
                    b.Dir = Beam::Direction::Up;
                    bCopy.Dir = Beam::Direction::Down;
                    beam2.emplace_back(b);
                    beam2.emplace_back(bCopy);
                }
                break;
            case '-':
                if (b.Dir == Beam::Direction::Left || b.Dir == Beam::Direction::Right) beam2.emplace_back(b);
                else
                {
                    Beam bCopy = b;
                    b.Dir = Beam::Direction::Left;
                    bCopy.Dir = Beam::Direction::Right;
                    beam2.emplace_back(b);
                    beam2.emplace_back(bCopy);
                }
                break;
            default:
                throw std::exception();
            }
        }

        std::swap(beams, beam2);
    }

    const auto result = std::accumulate(visited.begin(), visited.end(), 0UL);
    return result;
}

void PrintBoard(std::vector<bool> const& visited, uint32_t width, uint32_t height)
{
    for (auto y = 0UL; y < height; ++y)
    {
        for (auto x = 0UL; x < width; ++x)
        {
            std::cerr << (visited[y * width + x] ? '#' : '.');
        }
        std::cerr << std::endl;
    }
}

void Part1()
{
    std::string board;
    uint32_t height{};
    uint32_t width{};

    std::string input;
    while (std::getline(std::cin, input))
    {
        width = input.size();
        ++height;
        board.append(std::move(input));
    }

    Beam firstBeam { -1, 0, Beam::Direction::Right };
    const auto result = RunSimulation(board, width, height, firstBeam);
    std::cout << result << std::endl;
}

void Part2()
{
    std::string board;
    uint32_t height{};
    uint32_t width{};

    std::string input;
    while (std::getline(std::cin, input))
    {
        width = input.size();
        ++height;
        board.append(std::move(input));
    }

    uint32_t maxElement{};
    for (auto x = 0; x < (int32_t)width; ++x)
    {
        Beam downBeam { x, -1, Beam::Direction::Down };
        const auto downResult = RunSimulation(board, width, height, downBeam);
        maxElement = std::max(maxElement, downResult);

        Beam upBeam { x, (int)height, Beam::Direction::Up };
        const auto upResult = RunSimulation(board, width, height, upBeam);
        maxElement = std::max(maxElement, upResult);
    }

    for (auto y = 0; y < (int32_t)height; ++y)
    {
        Beam rightBeam { -1, y, Beam::Direction::Right };
        const auto rightResult = RunSimulation(board, width, height, rightBeam);
        maxElement = std::max(maxElement, rightResult);

        Beam leftBeam { (int)width, y, Beam::Direction::Left };
        const auto leftResult = RunSimulation(board, width, height, leftBeam);
        maxElement = std::max(maxElement, leftResult);
    }

    std::cout << maxElement << std::endl;
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include "HelpfulInfrastructure.h"

using namespace std; // I wouldn't normally do this
using namespace std::literals;

#pragma warning (disable: 4267) // size_t <-> uint
#pragma warning (disable: 4018) // signed/unsigned

namespace
{
    bool IsSeatOccupied(std::vector<uint8_t>& seating, int x, int y, uint32_t width, uint32_t height)
    {
        if (x < 0) return false;
        if (y < 0) return false;
        if (x >= width) return false;
        if (y >= height) return false;

        const size_t index = (y * width) + x;
        return seating[index] == 2;
    }

    uint8_t ComputeSeat(std::vector<uint8_t>& seating, size_t index, uint32_t width, uint32_t height)
    {
        if (seating[index] == 0) return 0; // don't bother computing floor

        auto y = index / width;
        auto x = (index - (y * width));

        const auto countNearby = 
            IsSeatOccupied(seating, x - 1, y - 1, width, height) +
            IsSeatOccupied(seating, x, y - 1, width, height) +
            IsSeatOccupied(seating, x + 1, y - 1, width, height) +
            IsSeatOccupied(seating, x - 1, y, width, height) +
            IsSeatOccupied(seating, x + 1, y, width, height) +
            IsSeatOccupied(seating, x - 1, y + 1, width, height) +
            IsSeatOccupied(seating, x, y + 1, width, height) +
            IsSeatOccupied(seating, x + 1, y + 1, width, height);
        
        if (seating[index] == 1 && countNearby == 0)
        {
            return 2;
        }
        else if (seating[index] == 2 && countNearby >= 4)
        {
            return 1;
        }
        return seating[index];
    }

    void DebugOutput(std::vector<uint8_t>& seating, uint32_t width)
    {
        for (auto i = 0UL; i < seating.size(); ++i)
        {
            std::cerr << (seating[i] == 0 ? '.' : (seating[i] == 1 ? 'L' : '#'));
            if ((i + 1) % width == 0) std::cerr << std::endl;
        }
        std::cerr << std::endl << std::endl;
    }

    void Part1()
    {
        // 0 = floor, 1 = empty east, 2 = occupied seat
        std::vector<uint8_t> seating{};
        uint32_t width{};
        uint32_t height{};

        std::string input;
        while (std::getline(std::cin, input))
        {
            width = input.length();
            for (const auto letter : input)
            {
                seating.emplace_back(letter == 'L' ? (uint8_t)1 : (uint8_t)0);
            }
        }
        height = seating.size() / width;

        std::vector<uint8_t> seatingCopy;
        do
        {
            seatingCopy = seating;
            // DebugOutput(seating, width);

            for (auto i = 0UL; i < seating.size(); ++i)
            {
                seating[i] = ComputeSeat(seatingCopy, i, width, height);
            }
        } while (seating != seatingCopy);
        
        uint32_t occupiedAtEnd{};
        for (const auto i : seating)
        {
            if (i == 2) ++occupiedAtEnd;
        }

        std::cout << occupiedAtEnd << std::endl;
    }

    bool IsSeatOccupiedDirection(std::vector<uint8_t>& seating, int x, int y, uint32_t width, uint32_t height, int dx, int dy)
    {
        x += dx;
        y += dy;
        while ((x >= 0) && (x < width) && (y >= 0) && (y < height))
        {
            const size_t index = (y * width) + x;
            if (seating[index] == 2)
            {
                return true;
            }
            else if (seating[index] == 1)
            {
                return false;
            }
            x += dx;
            y += dy;
        }

        return false;
    }

    uint8_t ComputeSeatPart2(std::vector<uint8_t>& seating, size_t index, uint32_t width, uint32_t height)
    {
        if (seating[index] == 0) return 0; // don't bother computing floor

        auto y = index / width;
        auto x = (index - (y * width));

        const auto countNearby = 
            IsSeatOccupiedDirection(seating, x, y, width, height, -1, -1) +
            IsSeatOccupiedDirection(seating, x, y, width, height, 0, -1) +
            IsSeatOccupiedDirection(seating, x, y, width, height, +1, -1) +
            IsSeatOccupiedDirection(seating, x, y, width, height, -1, 0) +
            IsSeatOccupiedDirection(seating, x, y, width, height, +1, 0) +
            IsSeatOccupiedDirection(seating, x, y, width, height, -1, +1) +
            IsSeatOccupiedDirection(seating, x, y, width, height, 0, +1) +
            IsSeatOccupiedDirection(seating, x, y, width, height, +1, +1);
        
        if (seating[index] == 1 && countNearby == 0)
        {
            return 2;
        }
        else if (seating[index] == 2 && countNearby >= 5)
        {
            return 1;
        }
        return seating[index];
    }

    void Part2()
    {
        // 0 = floor, 1 = empty east, 2 = occupied seat
        std::vector<uint8_t> seating{};
        uint32_t width{};
        uint32_t height{};

        std::string input;
        while (std::getline(std::cin, input))
        {
            width = input.length();
            for (const auto letter : input)
            {
                seating.emplace_back(letter == 'L' ? (uint8_t)1 : (uint8_t)0);
            }
        }
        height = seating.size() / width;

        std::vector<uint8_t> seatingCopy;
        do
        {
            seatingCopy = seating;
            // DebugOutput(seating, width);

            for (auto i = 0UL; i < seating.size(); ++i)
            {
                seating[i] = ComputeSeatPart2(seatingCopy, i, width, height);
            }
        } while (seating != seatingCopy);
        
        uint32_t occupiedAtEnd{};
        for (const auto i : seating)
        {
            if (i == 2) ++occupiedAtEnd;
        }

        std::cout << occupiedAtEnd << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
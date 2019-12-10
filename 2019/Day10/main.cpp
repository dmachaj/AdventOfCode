#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

namespace
{
    int32_t LowestCommonDenominator(int32_t first, int32_t second)
    {
        while (true)
        {
            if (first == 0) { return second; }
            second %= first;
            if (second == 0) { return first; }
            first %= second;
        }
    }

    uint32_t CheckVisibility(std::vector<bool> const& asteroidField, int32_t rows, int32_t columns, int32_t startX, int32_t startY)
    {
        uint32_t visibleOthers{};
        for (auto x = 0; x < columns; ++x)
        {
            for (auto y = 0; y < rows; ++y)
            {
                if ((x == startX) && (y == startY)) { continue; }

                if (asteroidField[y * columns + x])
                {
                    auto rise = startY - y;
                    auto run = startX - x;

                    const auto lcd = std::abs(LowestCommonDenominator(rise, run));
                    rise /= lcd;
                    run /= lcd;

                    int32_t tempX = x;
                    int32_t tempY = y;
                    for (auto i = 0;; ++i)
                    {
                        tempX += run;
                        tempY += rise;
                        if ((tempX == startX) && (tempY == startY))
                        {
                            visibleOthers++;
                            break;
                        }
                        else if (asteroidField[tempY * columns + tempX])
                        {
                            // path is blocked
                            break;
                        }
                    }
                }
            }
        }
        return visibleOthers;
    }

    void Part1()
    {
        std::vector<bool> asteroidField{};
        std::string input;
        int32_t rows{};
        while (std::getline(std::cin, input))
        {
            for (const auto& cell : input)
            {
                if (cell == '#')
                {
                    asteroidField.emplace_back(true);
                }
                else
                {
                    asteroidField.emplace_back(false);
                }
            }
            ++rows;
        }
        int32_t columns = (int32_t)asteroidField.size() / rows;

        uint32_t maxVisible{};
        for (auto x = 0; x < columns; ++x)
        {
            for (auto y = 0; y < rows; ++y)
            {
                if (asteroidField[y * columns + x])
                {
                    const auto visible = CheckVisibility(asteroidField, rows, columns, x, y);
                    if (visible > maxVisible)
                    {
                        maxVisible = visible;
                    }
                }
                // std::cerr << x << y << " ";
            }
            // std::cerr << std::endl;
        }

        // for (const auto row : asteroidField)
        // {
        //     for (const auto cell: row)
        //     {
        //         std::cerr << cell;
        //     }
        //     std::cerr << std::endl;
        // }

        std::cout << maxVisible << std::endl;
    }

    void Part2()
    {
        std::cout << 0 << std::endl;
    }
}

int main()
{
    Part1();
    // Part2();
    return 0;
}
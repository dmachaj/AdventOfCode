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

    template<typename TLambda>
    void CheckVisibility(std::vector<bool> const& asteroidField, int32_t rows, int32_t columns, int32_t startX, int32_t startY, TLambda callback)
    {
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
                            callback(x, y);
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
    }

    struct AsteroidField
    {
        std::vector<bool> asteroidField{};
        int32_t rows{};
        int32_t columns{};
    };

    AsteroidField ReadInput()
    {
        AsteroidField result{};
        std::string input;
        while (std::getline(std::cin, input))
        {
            for (const auto& cell : input)
            {
                if (cell == '#')
                {
                    result.asteroidField.emplace_back(true);
                }
                else
                {
                    result.asteroidField.emplace_back(false);
                }
            }
            ++result.rows;
        }
        result.columns = (int32_t)result.asteroidField.size() / result.rows;
        return result;
    }

    struct VisibilityResult
    {
        std::pair<int32_t, int32_t> bestLocation{};
        uint32_t numVisible;
    };

    VisibilityResult FindBestAsteroid(const AsteroidField& data)
    {
        uint32_t maxVisible{};
        std::pair<int32_t, int32_t> bestLocation{};
        for (auto x = 0; x < data.columns; ++x)
        {
            for (auto y = 0; y < data.rows; ++y)
            {
                if (data.asteroidField[y * data.columns + x])
                {
                    uint32_t countVisible{};
                    CheckVisibility(data.asteroidField, data.rows, data.columns, x, y,
                        [&](int32_t x, int32_t y)
                        {
                            x;y;
                            countVisible++;
                        });
                    if (countVisible > maxVisible)
                    {
                        maxVisible = countVisible;
                        bestLocation = std::make_pair(x, y);
                    }
                }
            }
        }

        return { bestLocation, maxVisible };
    }

    void Part1()
    {
        const auto data = ReadInput();
        const auto result = FindBestAsteroid(data);
        std::cout << result.numVisible << std::endl;
    }

    double VectorAngle(int32_t x, int32_t y)
    {
        // if (x == 0) // special cases
        // {
        //     return (y > 0) ? 90 : 270;
        // }
        // else if (y == 0) // special cases
        // {
        //     return (x >= 0) ? 0 : 180;
        // }
        
        double magnitude = sqrt((y * y) + (x * x));
        double riseUnit = (double)y / magnitude;
        double runUnit = (double)x / magnitude;
        // const auto angle = (int32_t)((atan2(riseUnit, runUnit) / 3.14) * 180);
        return (atan2(riseUnit, runUnit) / 3.14) * 180;
        // if (x < 0 && y < 0) // quadrant Ⅲ
        // {
        //     return 180 + angle;
        // }
        // else if (x < 0) // quadrant Ⅱ
        // {
        //     return 180 + angle; // it actually substracts
        // }
        // else if (y < 0) // quadrant Ⅳ
        // {
        //     return 270 + (90 + angle); // it actually substracts
        // }
        // return angle;
    }

    void Part2()
    {
        auto data = ReadInput();
        const auto result = FindBestAsteroid(data);
        std::cerr << "The best asteroid is " << result.bestLocation.first << "," << result.bestLocation.second << std::endl;
        uint32_t asteroidsZapped{};
        double currentAngle{90.0}; // 90 degrees is straight up on the unit circle
        std::pair<int32_t, int32_t> currentZapTarget{};
        while (asteroidsZapped < 200)
        {
            double smallestAngleDiff{360};
            CheckVisibility(data.asteroidField, data.rows, data.columns, result.bestLocation.first, result.bestLocation.second,
                [&](int32_t x, int32_t y)
                {
                    auto rise = (y - result.bestLocation.second) * -1; // negate because euclidean coords count up from the center not down from the top
                    auto run = x - result.bestLocation.first;

                    const auto angle = VectorAngle(run, rise);

                    auto angleDiff = (currentAngle + 0.1) - angle;
                    if (angleDiff < 0)
                    {
                        angleDiff += 360;
                    }

                    if (angleDiff < smallestAngleDiff)
                    {
                        smallestAngleDiff = angleDiff;
                        currentZapTarget = std::make_pair(x, y);
                    }
                });
            
            currentAngle -= (smallestAngleDiff + 0.01);
            if (currentAngle < 0)
            {
                currentAngle += 360;
            }
            data.asteroidField[currentZapTarget.second * data.columns + currentZapTarget.first] = false; // ZAP!
            asteroidsZapped++;
            std::cerr << asteroidsZapped << "th asteroid at " << currentZapTarget.first << "," << currentZapTarget.second << std::endl;
        }
        std::cout << (currentZapTarget.first * 100) + currentZapTarget.second << std::endl;
    }
}

int main()
{
    // Part1();
    Part2();
    // return 0;
}
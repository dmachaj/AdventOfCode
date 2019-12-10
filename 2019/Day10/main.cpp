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

    void Part2()
    {
        auto data = ReadInput();
        const auto result = FindBestAsteroid(data);
        std::cerr << "The best asteroid is " << result.bestLocation.first << "," << result.bestLocation.second << std::endl;
        uint32_t asteroidsZapped{};
        while (asteroidsZapped < 200)
        {
            struct HitData
            {
                std::pair<int32_t, int32_t> coord;
                double angle;
            };
            std::vector<HitData> currentZapTargets{};

            CheckVisibility(data.asteroidField, data.rows, data.columns, result.bestLocation.first, result.bestLocation.second,
                [&](int32_t x, int32_t y)
                {
                    auto rise = (y - result.bestLocation.second) * -1; // negate because euclidean coords count up from the center not down from the top
                    auto run = x - result.bestLocation.first;

                    const auto angle = (atan2(rise, run) / 3.1415926535897932384) * 180;

                    HitData match {std::make_pair(x, y), angle};
                    currentZapTargets.emplace_back(std::move(match));
                });

            std::sort(currentZapTargets.begin(), currentZapTargets.end(), [](HitData& first, HitData& second) -> bool
            {
                auto firstDiff = 90.0 - first.angle;
                if (firstDiff < 0) { firstDiff += 360; }
                auto secondDiff = 90.0 - second.angle;
                if (secondDiff < 0) { secondDiff += 360; }
                return (firstDiff < secondDiff);
            });

            for (const auto& zaps: currentZapTargets)
            {
                data.asteroidField[zaps.coord.second * data.columns + zaps.coord.first] = false; // ZAP!
                asteroidsZapped++;
                // std::cerr << asteroidsZapped << "th asteroid at " << zaps.coord.first << "," << zaps.coord.second << std::endl;
                if (asteroidsZapped >= 200)
                {
                    std::cerr << "The 200th asteroid is at " << zaps.coord.first << "," << zaps.coord.second << std::endl;
                    std::cout << (zaps.coord.first * 100) + zaps.coord.second << std::endl;
                    break;
                }
            }
        }
    }
}

int main()
{
    // Part1();
    Part2();
    return 0;
}
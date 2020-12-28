#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "HelpfulInfrastructure.h"
#include "../Intcode/Intcode.h"

namespace
{
    void Part1()
    {
        auto state = Intcode::ParseProgram();

        constexpr auto c_width = 50;
        constexpr auto c_height = 50;
        std::array<bool, c_width * c_height> results{};

        for (auto y = 0UL; y < c_height; ++y)
        {
            bool anyXMatches{false};
            for (auto x = 0UL; x < c_width; ++x)
            {
                Intcode::ProgramState stateCopy {state.program, 0, 0};
                std::vector<uint64_t> programInput{x, y};
                const auto result = Intcode::ExecuteProgram(stateCopy, programInput);
                if ((result.size() > 0) && (result[0] != 0))
                {
                    results[y * c_width + x] = true;
                    anyXMatches = true;
                }
                else if ((results.size() > 0) && (result[0] == 0) && anyXMatches)
                {
                    break;
                }
            }
        }

        // for (auto y = 0UL; y < c_height; ++y)
        // {
        //     for (auto x = 0UL; x < c_width; ++x)
        //     {
        //         std::cerr << (results[y * c_width + x] ? "#" : ".");
        //     }
        //     std::cerr << std::endl;
        // }

        uint32_t affectedSquares{};
        for (const auto square : results)
        {
            if (square) { affectedSquares++; }
        }
        std::cout << affectedSquares << std::endl;
    }

    static std::unordered_map<uint32_t, bool> resultCache{};
    bool IsSquareAffected(const std::vector<int64_t>& program, const uint32_t x, const uint32_t y)
    {
        const auto cache = resultCache.find((x * 10000) + y);
        if (cache != resultCache.end())
        {
            return cache->second;
        }

        Intcode::ProgramState state {program, 0, 0};
        std::vector<uint64_t> programInput{x, y};
        const auto intCodeResult = Intcode::ExecuteProgram(state, programInput);
        const bool result = ((intCodeResult.size() != 0) && (intCodeResult[0] != 0));
        resultCache.emplace((x * 10000) + y, result);
        return result;
    }

    uint32_t c_width = 10000;
    uint32_t c_height = 10000;
    constexpr uint32_t necessaryWidth{ 100 };
    constexpr uint32_t necessaryHeight{ 100 };
    bool DoesSquareFitWithin(const std::vector<int64_t>& program, const uint32_t x, const uint32_t y)
    {
        for (auto iy = y; iy < (y + necessaryHeight); ++iy)
        {
            for (auto ix = x; ix < (x + necessaryWidth); ++ix)
            {
                if (!IsSquareAffected(program, ix, iy))
                {
                    // std::cerr << "Square starting at (" << x << "," << y << ") failed at " << ((iy - y != 0) ? necessaryWidth : (ix - x)) << " wide and " << (iy - y) << " tall." << std::endl;
                    return false;
                }
            }
        }
        return true;
    }

    void Part2()
    {
        auto state = Intcode::ParseProgram();

        uint32_t minX = 0UL;
        for (auto y = 0UL; y < c_height; ++y)
        {
            for (auto x = minX; x < c_width; ++x)
            {
                if (IsSquareAffected(state.program, x, y))
                {
                    if ((x - 1 > 0) && !IsSquareAffected(state.program, x - 1, y))
                    {
                        minX = x;
                    }

                    if (DoesSquareFitWithin(state.program, x, y))
                    {
                        const auto finalResult = (x * 10000) + y;
                        std::cout << finalResult << std::endl;
                        return;
                    }
                    else if (!IsSquareAffected(state.program, x + 1, y))
                    {
                        break;
                    }
                }
            }
        }

        std::cerr << "Did not find a match :(" << std::endl;
        std::cout << 0 << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
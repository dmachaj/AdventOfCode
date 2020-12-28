#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <unordered_map>
#include "HelpfulInfrastructure.h"

namespace
{
    bool IsValidPassword(uint32_t input)
    {
        const uint32_t a = (input / 100000) % 10;
        const uint32_t b = (input / 10000) % 10;
        const uint32_t c = (input / 1000) % 10;
        const uint32_t d = (input / 100) % 10;
        const uint32_t e = (input / 10) % 10;
        const uint32_t f = (input) % 10;

        if ((a == b) || (b == c) || (c == d) || (d == e) || (e == f))
        {
            if ((a <= b) && (b <= c) && (c <= d) && (d <= e) && (e <= f))
            {
                return true;
            }
        }

        // std::cerr << a << " " << b << " " << c << " " << d << " " << e << " " << f << std::endl;
        return false;
    }

    bool IsValidPasswordPart2(uint32_t input)
    {
        const uint32_t a = (input / 100000) % 10;
        const uint32_t b = (input / 10000) % 10;
        const uint32_t c = (input / 1000) % 10;
        const uint32_t d = (input / 100) % 10;
        const uint32_t e = (input / 10) % 10;
        const uint32_t f = (input) % 10;

        if ((a <= b) && (b <= c) && (c <= d) && (d <= e) && (e <= f))
        {
            std::array<uint32_t, 10> digits{};
            digits[a]++;
            digits[b]++;
            digits[c]++;
            digits[d]++;
            digits[e]++;
            digits[f]++;

            for (const auto digit : digits)
            {
                if (digit == 2)
                {
                    return true;
                }
            }
        }

        // std::cerr << a << " " << b << " " << c << " " << d << " " << e << " " << f << std::endl;
        return false;
    }

    void Part1()
    {
        std::string input;
        std::getline(std::cin, input, '-');
        const uint32_t min = std::atoi(input.c_str());
        std::getline(std::cin, input);
        const uint32_t max = std::atoi(input.c_str());

        uint32_t validPasswords{};
        for(uint32_t i = min; i < max; ++i)
        {
            if (IsValidPassword(i))
            {
                ++validPasswords;
            }
        }

        std::cout << validPasswords;
    }

    void Part2()
    {
        std::string input;
        std::getline(std::cin, input, '-');
        const uint32_t min = std::atoi(input.c_str());
        std::getline(std::cin, input);
        const uint32_t max = std::atoi(input.c_str());

        uint32_t validPasswords{};
        for(uint32_t i = min; i < max; ++i)
        {
            if (IsValidPasswordPart2(i))
            {
                ++validPasswords;
            }
        }

        std::cout << validPasswords;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
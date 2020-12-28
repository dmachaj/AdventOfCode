#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "HelpfulInfrastructure.h"

#pragma warning (disable: 4267) // size_t conversion

namespace
{
    void Part1()
    {
        std::string input;
        std::vector<char> map{};
        uint32_t width{};
        while (std::getline(std::cin, input))
        {
            width = input.length();
            for (const auto letter : input)
            {
                map.emplace_back(letter);
            }
        }
        const uint32_t height = map.size() / width;

        for (auto y = 0UL; y < height; ++y)
        {
            for (auto x = 0UL; x < width; ++x)
            {
                std::cerr << map[y * width + x];
            }
            std::cerr << std::endl;
        }
        std::cerr << std::endl;
        std::cout << 0 << std::endl;
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
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

namespace
{
    void Part1()
    {
        uint32_t width, height;
        std::cin >> width >> height;
        std::string input;
        std::cin >> input;

        uint32_t layerSize = width * height;
        uint32_t layerCount = (uint32_t)input.length() / layerSize;

        uint32_t minCountZero{0xFFFFFFFF};
        uint32_t result{};
        for (uint32_t i = 0; i < layerCount; ++i)
        {
            uint32_t countZero{};
            uint32_t countOne{};
            uint32_t countTwo{};
            for (uint32_t j = 0; j < layerSize; ++j)
            {
                const auto current = input[(i * layerSize) + j];
                switch (current)
                {
                    case '0':
                        countZero++;
                        break;
                    case '1':
                        countOne++;
                        break;
                    case '2':
                        countTwo++;
                        break;
                }
            }

            if (countZero < minCountZero)
            {
                minCountZero = countZero;
                result = countOne * countTwo;
            }
        }

        std::cout << result << std::endl;
    }

    void Part2()
    {
        uint32_t width, height;
        std::cin >> width >> height;
        std::string input;
        std::cin >> input;

        uint32_t layerSize = width * height;
        uint32_t layerCount = (uint32_t)input.length() / layerSize;

        std::vector<char> finalPixels(width * height, '2');
        for (uint32_t i = 0; i < layerCount; ++i)
        {
            for (uint32_t j = 0; j < layerSize; ++j)
            {
                const auto current = input[(i * layerSize) + j];
                if ((finalPixels[j] == '2') && (current != '2'))
                {
                    finalPixels[j] = current;
                }
            }
        }

        for (uint32_t i = 0; i < finalPixels.size(); ++i)
        {
            switch (finalPixels[i])
            {
                case '0':
                    std::cout << '0';
                    break;
                case '1':
                    std::cout << '1';
                    break;
                case '2':
                    std::cout << " ";
                    break;
            }
            if ((i + 1) % width == 0) { std::cout << std::endl; }
        }
    }
}

int main()
{
    // Part1();
    Part2();
    return 0;
}
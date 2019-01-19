#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>

int main()
{
    constexpr uint32_t c_boardSize{1000};
    std::array<std::array<uint32_t, c_boardSize>, c_boardSize> board{};

    std::string input;
    while (std::getline(std::cin, input))
    {
        std::istringstream tempStream(input);
        char pound, at, comma, colon, by;
        uint32_t id, x, y, width, height;
        tempStream >> pound >> id >> at >> x >> comma >> y >> colon >> width >> by >> height;
        // std::cout << "Id " << id << ": [" << x << "," << y << "," << width << "," << height << "]" << std::endl;
    }

    uint32_t total{};
    for (auto i = 0UL; i < c_boardSize; ++i)
    {
        for (auto j = 0UL; j < c_boardSize; ++j)
        {
            if (board[i][j] > 1)
            {
                total++;
            }
        }
    }
    std::cout << "The answer is: " << total << std::endl;

    return 0;
}
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <set>
#include <string>
#include <sstream>
#include <vector>

int main()
{
    constexpr uint32_t c_boardSize{1000};
    static std::array<std::array<std::vector<uint32_t>, c_boardSize>, c_boardSize> boardOwners{}; // exceeds max stack size, so must be static

    std::set<uint32_t> allOwners{};
    std::string input;
    while (std::getline(std::cin, input))
    {
        std::istringstream tempStream(input);
        char pound, at, comma, colon, by;
        uint32_t id, x, y, width, height;
        tempStream >> pound >> id >> at >> x >> comma >> y >> colon >> width >> by >> height;
        allOwners.insert(id);

        for (auto i = x; i < (x + width); ++i)
        {
            for (auto j = y; j < (y + height); ++j)
            {
                boardOwners[i][j].push_back(id);
            }
        }
        // std::cout << "Id " << id << ": [" << x << "," << y << "," << width << "," << height << "]" << std::endl;
    }

    for (auto i = 0UL; i < c_boardSize; ++i)
    {
        for (auto j = 0UL; j < c_boardSize; ++j)
        {
            if (boardOwners[i][j].size() > 1)
            {
                for (auto owner : boardOwners[i][j])
                {
                    allOwners.erase(owner);
                }
            }
        }
    }

    if (allOwners.size() != 1)
    {
        std::cout << "I fucked up" << std::endl;
    }
    else
    {
        std::cout << *allOwners.begin() << std::endl;
    }

    return 0;
}
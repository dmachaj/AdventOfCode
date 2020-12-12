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

namespace
{
    void Part1()
    {
        std::vector<string> commands{};

        std::string input;
        while (std::getline(std::cin, input))
        {
            commands.emplace_back(move(input));
        }

        // x increasing == east
        // y increasing == south
        int x{};
        int y{};
        int dx{1};
        int dy{0};

        for (const auto& command : commands)
        {
            char type = command[0];
            int amount = std::atoi(command.substr(1).c_str());

            // Remap L to R so that the horrible hacky code below doesn't need as much near-duplication.
            if (type == 'L')
            {
                if (amount == 90)
                {
                    amount = 270;
                }
                else if (amount == 270)
                {
                    amount = 90;
                }
                type = 'R';
            }

            switch (type)
            {
                case 'N':
                    y -= amount;
                    break;
                case 'S':
                    y += amount;
                    break;
                case 'E':
                    x += amount;
                    break;
                case 'W':
                    x -= amount;
                    break;
                case 'F':
                    x += (dx * amount);
                    y += (dy * amount);
                    break;
                case 'R':
                    if (amount == 180)
                    {
                        dx *= -1;
                        dy *= -1;
                        break;
                    }

                    if (dx == 1)
                    {
                        dx = 0;
                        if (amount == 90)
                        {
                            dy = 1;
                        }
                        else if (amount == 270)
                        {
                            dy = -1;
                        }
                        
                    }
                    else if (dx == -1)
                    {
                        dx = 0;
                        if (amount == 90)
                        {
                            dy = -1;
                        }
                        else if (amount == 270)
                        {
                            dy = 1;
                        }
                    }
                    else if (dy == -1)
                    {
                        dy = 0;
                        if (amount == 90)
                        {
                            dx = 1;
                        }
                        else if (amount == 270)
                        {
                            dx = -1;
                        }
                    }
                    else if (dy == 1)
                    {
                        dy = 0;
                        if (amount == 90)
                        {
                            dx = -1;
                        }
                        else if (amount == 270)
                        {
                            dx = 1;
                        }
                    }
                    break;
            }
        }

        std::cout << (std::abs(x) + std::abs(y)) << std::endl;
    }

    void Part2()
    {
        std::vector<string> commands{};

        std::string input;
        while (std::getline(std::cin, input))
        {
            commands.emplace_back(move(input));
        }

        // x increasing == east
        // y increasing == south
        int x{};
        int y{};
        int waypointX{10};
        int waypointY{-1};

        for (const auto& command : commands)
        {
            char type = command[0];
            int amount = std::atoi(command.substr(1).c_str());

            // Remap L to R so that the horrible hacky code below doesn't need as much near-duplication.
            if (type == 'L')
            {
                if (amount == 90)
                {
                    amount = 270;
                }
                else if (amount == 270)
                {
                    amount = 90;
                }
                type = 'R';
            }

            switch (type)
            {
                case 'N':
                    waypointY -= amount;
                    break;
                case 'S':
                    waypointY += amount;
                    break;
                case 'E':
                    waypointX += amount;
                    break;
                case 'W':
                    waypointX -= amount;
                    break;
                case 'F':
                    x += (waypointX * amount);
                    y += (waypointY * amount);
                    break;
                case 'R':
                    if (amount == 180)
                    {
                        waypointX *= -1;
                        waypointY *= -1;
                        break;
                    }

                    if ((waypointX > 0) && (amount == 270))
                    {
                        waypointX *= -1;
                    }
                    else if ((waypointX < 0) && (amount == 270))
                    {
                        waypointX *= -1;
                    }

                    if ((waypointY > 0) && (amount == 90))
                    {
                        waypointY *= -1;
                    }
                    else if ((waypointY < 0) && (amount == 90))
                    {
                        waypointY *= -1;
                    }
                    std::swap(waypointX, waypointY);

                    break;
            }
        }

        std::cout << (std::abs(x) + std::abs(y)) << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
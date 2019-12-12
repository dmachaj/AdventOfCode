#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#pragma warning (disable: 4267) // size_t to int

namespace
{
    void Part1()
    {
        std::string input;
        std::getline(std::cin, input);
        uint32_t stepCount = std::atoi(input.c_str());
        std::cerr << "Will run simulation for N steps: " << stepCount << std::endl;

        struct MoonData
        {
            int32_t x{};
            int32_t y{};
            int32_t z{};

            int32_t dx{};
            int32_t dy{};
            int32_t dz{};
        };
        std::array<MoonData, 4> moons{};

        for (auto& moon : moons)
        {
            std::getline(std::cin, input);
            uint32_t xPos = input.find('=') + 1;
            uint32_t xEnd = input.find(',', xPos);
            moon.x = std::atoi(input.substr(xPos, (xEnd - xPos)).c_str());

            uint32_t yPos = input.find('=', xEnd) + 1;
            uint32_t yEnd = input.find(',', yPos);
            moon.y = std::atoi(input.substr(yPos, (yEnd - yPos)).c_str());

            uint32_t zPos = input.find('=', yEnd) + 1;
            uint32_t zEnd = input.find('>', zPos);
            moon.z = std::atoi(input.substr(zPos, (zEnd - zPos)).c_str());

            std::cerr << "Moon xyz = " << moon.x << " " << moon.y << " " << moon.z << std::endl;
        }

        for (auto i = 0UL; i < stepCount; ++i)
        {
            // Update velocity
            for (auto iMoon = 0UL; iMoon < moons.size(); ++iMoon)
            {
                for (auto otherMoon = 0UL; otherMoon < moons.size(); ++otherMoon)
                {
                    if (iMoon == otherMoon) { continue; }

                    if (moons[iMoon].x != moons[otherMoon].x)
                    {
                        if (moons[iMoon].x >= moons[otherMoon].x)
                        {
                            moons[iMoon].dx--;
                            // moons[otherMoon].dx++;
                        }
                        else
                        {
                            moons[iMoon].dx++;
                            // moons[otherMoon].dx--;
                        }
                        
                    }

                    if (moons[iMoon].y != moons[otherMoon].y)
                    {
                        if (moons[iMoon].y >= moons[otherMoon].y)
                        {
                            moons[iMoon].dy--;
                            // moons[otherMoon].dy++;
                        }
                        else
                        {
                            moons[iMoon].dy++;
                            // moons[otherMoon].dy--;
                        }
                        
                    }
                    
                    if (moons[iMoon].z != moons[otherMoon].z)
                    {
                        if (moons[iMoon].z >= moons[otherMoon].z)
                        {
                            moons[iMoon].dz--;
                            // moons[otherMoon].dz++;
                        }
                        else
                        {
                            moons[iMoon].dz++;
                            // moons[otherMoon].dz--;
                        }
                        
                    }
                }
            }

            // Update position
            for (auto& moon : moons)
            {
                moon.x += moon.dx;
                moon.y += moon.dy;
                moon.z += moon.dz;
            }
        }

        uint32_t totalEnergy{};
        for (const auto& moon : moons)
        {
            const auto potential = std::abs(moon.x) + std::abs(moon.y) + std::abs(moon.z);
            const auto kinetic = std::abs(moon.dx) + std::abs(moon.dy) + std::abs(moon.dz);
            totalEnergy += (potential * kinetic);
        }

        std::cout << totalEnergy << std::endl;
    }

        struct MoonData
        {
            int32_t x{};
            int32_t y{};
            int32_t z{};

            int32_t dx{};
            int32_t dy{};
            int32_t dz{};

        };
    
        bool operator== (const MoonData& first, const MoonData& second)
        {
            return (first.x == second.x) && (first.y == second.y) && (first.z == second.z) &&
                   (first.dx == second.dx) && (first.dy == second.dy) && (first.dz == second.dz);
        }

    void Part2()
    {
        std::string input;
        std::array<MoonData, 4> moons{};
        for (auto& moon : moons)
        {
            std::getline(std::cin, input);
            uint32_t xPos = input.find('=') + 1;
            uint32_t xEnd = input.find(',', xPos);
            moon.x = std::atoi(input.substr(xPos, (xEnd - xPos)).c_str());

            uint32_t yPos = input.find('=', xEnd) + 1;
            uint32_t yEnd = input.find(',', yPos);
            moon.y = std::atoi(input.substr(yPos, (yEnd - yPos)).c_str());

            uint32_t zPos = input.find('=', yEnd) + 1;
            uint32_t zEnd = input.find('>', zPos);
            moon.z = std::atoi(input.substr(zPos, (zEnd - zPos)).c_str());

            std::cerr << "Moon xyz = " << moon.x << " " << moon.y << " " << moon.z << std::endl;
        }

        const auto moonsCopy = moons;
        uint64_t cycleCount{};
        while (true)
        {
            // Update velocity
            for (auto iMoon = 0UL; iMoon < moons.size(); ++iMoon)
            {
                for (auto otherMoon = 0UL; otherMoon < moons.size(); ++otherMoon)
                {
                    if (iMoon == otherMoon) { continue; }

                    if (moons[iMoon].x != moons[otherMoon].x)
                    {
                        if (moons[iMoon].x >= moons[otherMoon].x)
                        {
                            moons[iMoon].dx--;
                            // moons[otherMoon].dx++;
                        }
                        else
                        {
                            moons[iMoon].dx++;
                            // moons[otherMoon].dx--;
                        }
                        
                    }

                    if (moons[iMoon].y != moons[otherMoon].y)
                    {
                        if (moons[iMoon].y >= moons[otherMoon].y)
                        {
                            moons[iMoon].dy--;
                            // moons[otherMoon].dy++;
                        }
                        else
                        {
                            moons[iMoon].dy++;
                            // moons[otherMoon].dy--;
                        }
                        
                    }
                    
                    if (moons[iMoon].z != moons[otherMoon].z)
                    {
                        if (moons[iMoon].z >= moons[otherMoon].z)
                        {
                            moons[iMoon].dz--;
                            // moons[otherMoon].dz++;
                        }
                        else
                        {
                            moons[iMoon].dz++;
                            // moons[otherMoon].dz--;
                        }
                        
                    }
                }
            }

            // Update position
            for (auto& moon : moons)
            {
                moon.x += moon.dx;
                moon.y += moon.dy;
                moon.z += moon.dz;
            }

            cycleCount++;
            if (moons == moonsCopy) { break; }
        }

        std::cout << cycleCount << std::endl;
    }
}

int main()
{
    // Part1();
    Part2();
    return 0;
}
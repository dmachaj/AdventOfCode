#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include "HelpfulInfrastructure.h"

void Part1()
{
    uint64_t totalFuel{};
    std::string input;
    while (std::getline(std::cin, input))
    {
        const auto mass = std::atoi(input.c_str());
        const auto fuel = (mass / 3) - 2;
        // std::cerr << "Mass: " << mass << " Fuel: " << fuel << std::endl;
        totalFuel += fuel;
    }

    std::cout << totalFuel << std::endl;
}

int64_t ComputeFuel(int64_t mass)
{
    const auto fuel = (mass / 3) - 2;
    return std::max(fuel, 0LL);
}

void Part2()
{
    uint64_t totalFuel{};
    std::string input;
    while (std::getline(std::cin, input))
    {
        const auto mass = std::atoi(input.c_str());
        const auto fuel = ComputeFuel(mass);
        totalFuel += fuel;

        auto fuelForFuel = ComputeFuel(fuel);
        while (fuelForFuel != 0)
        {
            // std::cerr << "FuelForFuel: " << fuelForFuel << " ";
            totalFuel += fuelForFuel;
            fuelForFuel = ComputeFuel(fuelForFuel);
        }

        // std::cerr << "Mass: " << mass << " Fuel: " << fuel << std::endl;
    }

    std::cout << totalFuel << std::endl;
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
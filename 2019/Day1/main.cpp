#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>

void part1()
{
    uint64_t totalFuel{};
    std::string input;
    while (std::getline(std::cin, input))
    {
        const auto mass = std::atoi(input.c_str());
        const auto fuel = (mass / 3) - 2;
        std::cerr << "Mass: " << mass << " Fuel: " << fuel << std::endl;
        totalFuel += fuel;
    }

    std::cout << totalFuel << std::endl;
}

int main()
{
    part1();
    return 0;
}
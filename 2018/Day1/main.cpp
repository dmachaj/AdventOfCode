#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>

int main()
{
    int64_t runningTotal{};
    std::string input;
    while (std::getline(std::cin, input))
    {
        const auto number = std::atoi(input.c_str());
        runningTotal += number;
        // std::cout << "Read in " << number << " running total " << runningTotal << std::endl;
    }
    std::cout << runningTotal << std::endl;
    return 0;
}
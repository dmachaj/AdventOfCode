#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "../../Includes/HelpfulInfrastructure.h"

#pragma warning (disable: 4267) // size_t <-> uint

namespace
{
    void Part1()
    {
        uint32_t validPasswords{};

        std::string input;
        while (std::getline(std::cin, input))
        {
            const auto dash = input.find("-");
            const auto colon = input.find(":");

            std::string_view minStr(input.c_str(), dash);
            std::string_view maxStr(input.c_str() + dash + 1, colon - dash - 2);
            char targetLetter = input[colon - 1];
            std::string_view password(input.c_str() + colon + 2);

            int min = std::atoi(minStr.data());
            int max = std::atoi(maxStr.data());

            int countOfLetter{};
            for (const auto letter : password)
            {
                if (letter == targetLetter)
                {
                    countOfLetter++;
                }
            }

            if ((countOfLetter >= min) && (countOfLetter <= max))
            {
                validPasswords++;
            }
            // std::cout << min << " " << max << " " << letter << " " << password.data() << std::endl;
        }

        std::cout << validPasswords << std::endl;
    }

    void Part2()
    {
        uint32_t validPasswords{};

        std::string input;
        while (std::getline(std::cin, input))
        {
            const auto dash = input.find("-");
            const auto colon = input.find(":");

            std::string_view minStr(input.c_str(), dash);
            std::string_view maxStr(input.c_str() + dash + 1, colon - dash - 2);
            char targetLetter = input[colon - 1];
            std::string_view password(input.c_str() + colon + 2);

            int min = std::atoi(minStr.data());
            int max = std::atoi(maxStr.data());

            bool valid = (password[min - 1] == targetLetter) ^ (password[max - 1] == targetLetter);
            if (valid) validPasswords++;
            // std::cout << min << " " << max << " " << letter << " " << password.data() << std::endl;
        }

        std::cout << validPasswords << std::endl;
    }
}

int main()
{
    RunPart1() ? Part1() : Part2();
    return 0;
}
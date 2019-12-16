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
        std::string input;
        std::getline(std::cin, input);
        const uint32_t iterationCount = std::atoi(input.c_str());

        std::getline(std::cin, input);
        std::vector<uint32_t> digits{};
        for (const auto& letter : input)
        {
            const std::string temp { letter };
            digits.emplace_back(std::atoi(temp.c_str()));
        }

        std::array<int32_t, 4> phase { 0, 1, 0, -1 };

        for (auto iterations = 0UL; iterations < iterationCount; ++iterations)
        {
            std::vector<uint32_t> digitsCopy = digits;
            for (auto i = 0UL; i < digits.size(); ++i)
            {
                std::vector<int32_t> phaseDigits{};
                auto phaseDigitOffset = 0UL;
                while (phaseDigits.size() < (digits.size() + 1))
                {
                    phaseDigits.insert(phaseDigits.end(), (i + 1), phase[phaseDigitOffset % phase.size()]);
                    phaseDigitOffset++;
                }
                phaseDigits.erase(phaseDigits.begin(), ++phaseDigits.begin()); // delete first entry

                int32_t runningTotal{};
                for (auto j = 0UL; j < digits.size(); ++j)
                {
                    runningTotal += digits[j] * phaseDigits[j];
                }
                digitsCopy[i] = std::abs(runningTotal) % 10;
            }

            digits = digitsCopy;
        }

        for (auto i = 0UL; i < 8; ++i)
        {
            std::cout << digits[i];
        }
        std::cout << std::endl;
    }

    void Part2()
    {
        std::cout << 0 << std::endl;
    }
}

int main()
{
    Part1();
    // Part2();
    return 0;
}
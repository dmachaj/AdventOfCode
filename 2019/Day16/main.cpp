#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <numeric>

namespace
{
    constexpr std::array<int32_t, 4> c_phase{ 0, 1, 0, -1 };
    void FFT(std::vector<uint32_t>& digits, uint32_t iterationCount)
    {
        for (auto iterations = 0UL; iterations < iterationCount; ++iterations)
        {
            std::vector<uint32_t> digitsCopy = digits;
            for (auto i = 0UL; i < digits.size(); ++i)
            {
                std::vector<int32_t> phaseDigits{};
                auto phaseDigitOffset = 0UL;
                while (phaseDigits.size() < (digits.size() + 1))
                {
                    phaseDigits.insert(phaseDigits.end(), (i + 1), c_phase[phaseDigitOffset % c_phase.size()]);
                    phaseDigitOffset++;
                }

                int32_t runningTotal{};
                for (auto j = 0UL; j < digits.size(); ++j)
                {
                    runningTotal += digits[j] * phaseDigits[j + 1];
                }
                digitsCopy[i] = std::abs(runningTotal) % 10;
            }

            digits = std::move(digitsCopy);
        }
    }

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

        FFT(digits, iterationCount);

        for (auto i = 0UL; i < 8; ++i)
        {
            std::cout << digits[i];
        }
        std::cout << std::endl;
    }

    void Part2()
    {
        constexpr uint32_t iterationCount = 100;
        constexpr uint32_t inputRepeatCount = 10000;

        std::string input;
        std::getline(std::cin, input);
        std::vector<uint32_t> digitsOriginal{};
        for (const auto& letter : input)
        {
            const std::string temp { letter };
            digitsOriginal.emplace_back(std::atoi(temp.c_str()));
        }

        uint32_t resultOffset{};
        {
            std::ostringstream resultOffsetStr{};
            for (auto i = 0UL; i < 7; ++i)
            {
                resultOffsetStr << digitsOriginal[i];
            }
            resultOffset = std::atoi(resultOffsetStr.str().c_str());
        }

        std::vector<uint32_t> digits{};
        for (auto i = 0UL; i < inputRepeatCount; ++i)
        {
            digits.insert(digits.end(), digitsOriginal.begin(), digitsOriginal.end());
        }

        // All digits before our offset will zero out so we can safely drop them, reducing memcpy costs.
        digits.erase(digits.begin(), digits.begin() + resultOffset);

        for (auto iterations = 0UL; iterations < iterationCount; ++iterations)
        {
            // Each entry is itself plus the sum of the remaining digits.  We can turn N^2 into N by simply
            // doing the addition in reverse.
            for (int32_t i = (int32_t)digits.size() - 2; i >= 0; --i)
            {
                digits[i] = (digits[i] + digits[i + 1]) % 10;
            }
        }

        for (auto i = 0UL; i < 8; ++i)
        {
            std::cout << digits[i];
        }
        std::cout << std::endl;
    }
}

int main()
{
    // Part1();
    Part2();
    return 0;
}